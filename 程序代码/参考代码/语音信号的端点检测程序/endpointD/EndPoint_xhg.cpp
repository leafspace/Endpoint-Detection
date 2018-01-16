#include <math.h>

#include "cst_lib.h"
#include "basop.h"
#include "EndPoint_xhg.h"



/*=====================================================
函数名称：	EndPointDetection
函数功能：	端点检测
输入参数：	EnergyArray		Frame_Num长的数组，存储每一个Frame的能量
			fRatioFreq		存储每一个Frame的低频域对数能量。
			nZeroPassArray	存储每一个Frame的过零次数。
			Frame_Num		总的帧数
			Energy_Th		噪声能量门限
输出参数：	nStartFrame		返回检测的起始端点
			nEndFrame		返回终止端点。

备注：		
基本思路：	根据观察数据, 元音段的过零率一般都要小于30, 而噪声的过零率一般都要小于100
			另外的两个特征都比较的稳健, 一般来说, 起伏不大
			//	由于对于孤立词, 不同的句子, 存在着一些比较异常的情况, 
			//  如从句子的开始就是语音, 或者是句子的托尾比较长, 直到句子末尾
			//	这样靠前后帧的能量作为端点就不太稳定, 而用很多句话自适应出来的能量
			//	门限如果各种话的背景不太一样, 那么这样的能量门限也是不可靠的
			//  所以根据低频能量的想法, 把总的能量也进行最大值的归一化, 其实实验发现这样也不行, 

====================================================*/
void	EndPointDetection(float *EnergyArray, short *nZeroPassArray, float *fRatioFreq, short Frame_Num, short &nStartFrame, short &nEndFrame, float Energy_Th)
{
	short	nCrudeStart, nCrudeEnd;				//粗略的起点和终点
	short	Wait_Duration = 0;					//统计计数用变量
	short	Wait_Duration1= 0;					//统计计数用变量
	short	wFrameCount;						//循环计数变量
	float	EnergyThreshUsed;					//可以调节的门限

	/**********************************************************
	<------------------粗判
	**********************************************************/
	EnergyThreshUsed =Energy_Th + 1.3f ; //由于Energy_Th是对数能量, 相对于能量域, 相当于Energy_Th * 27
	
	//确定元音段, 从第0帧开始
	for(wFrameCount=0; wFrameCount<Frame_Num; wFrameCount++)
	{
		//同时用过零率, 区段频域能量, 总的能量作为判断条件
		//可以先找到元音段, 如果针对汉语来做的话, 肯定是有元音段的, 
		if( (fRatioFreq[wFrameCount]>-4) && (nZeroPassArray[wFrameCount]<70) && (EnergyArray[wFrameCount]>EnergyThreshUsed))
		{
			Wait_Duration++;
			//如果连续有3帧满足条件, 即认为找到起始点
			if (Wait_Duration > 3)
				break;
		}
		else
			Wait_Duration = 0;
	}
	nCrudeStart = wFrameCount - 3;

	//如果在上述条件下没有找到起始帧nCrudeStart, 即端点的搜索已经到了最后一帧,则放宽条件
	if(wFrameCount == Frame_Num)  
	{
		Wait_Duration = 0;
		for (wFrameCount=0; wFrameCount<Frame_Num; wFrameCount++)
		{
			//此时仅仅用能量作为判断条件
			//相当于最大能量的0.135
			if (EnergyArray[wFrameCount] > (EnergyThreshUsed + 1 ))
			{
				Wait_Duration++;
				//如果连续有4帧满足条件, 即认为找到起始点
				if (Wait_Duration> 4)
					break;
			}
			else
				Wait_Duration = 0;
		}
		nCrudeStart = wFrameCount - 4;
	}

	//从语音的尾部, 倒数第4帧开始找结束帧nCrudeEnd
	Wait_Duration = 0;
	for (wFrameCount=Frame_Num-4; wFrameCount>nCrudeStart; wFrameCount--)
	{
		//同时用低频能量, 和总的能量作为条件, 过零率作为条件搜寻元音段, 即过零率<40
		if ((fRatioFreq[wFrameCount]>-4) && (EnergyArray[wFrameCount]>EnergyThreshUsed) && (nZeroPassArray[wFrameCount]<70))
		{
			Wait_Duration++;
			if (Wait_Duration> 2)
				break;
		}
		else
			Wait_Duration = 0;
	}
	nCrudeEnd = wFrameCount + 2;

	//如果在上述条件下没有找到结束帧nCrudeEnd, 则放宽条件
	if (wFrameCount == nCrudeStart)  
	{
		Wait_Duration = 0;
		for (wFrameCount=Frame_Num-1; wFrameCount>nCrudeStart; wFrameCount--)
		{
			//仅仅用能量作为标准来判断
			if (EnergyArray[wFrameCount] > (EnergyThreshUsed + 1))
			{
				Wait_Duration++;
				if (Wait_Duration > 2)
					break;
			}
			else
				Wait_Duration = 0;
		}
		nCrudeEnd = wFrameCount + 2;
	}

	//如果起始帧和结束帧的距离太小, 则将起始帧设为4, 结束帧设为最后一帧, 交给下一级
	if (nCrudeEnd - nCrudeStart < 2)
	{
		nCrudeStart = 4;
		nCrudeEnd = Frame_Num - 1;
	}
/***********************************************************
			细判
***********************************************************/
	//寻找结束帧, 从粗判的结束帧开始向后找静音段, 因为在
	//孤立词识别中, 它是必然会有一个结束帧的, 所有应该存在一定的静音段
	//通过数据发现, 过零率作为一个特征, 在找结束帧时, 不稳定, 不好用
	//EnergyThreshUsed = Energy_Th * 5
	EnergyThreshUsed = Energy_Th + 1.3f;
	Wait_Duration = 0;
	for (wFrameCount = nCrudeEnd; wFrameCount < Frame_Num; wFrameCount++)
	{
		//判断过零率, 如果过零率>100, 则认为是轻音, 而不是静音
		if((nZeroPassArray[wFrameCount] < 100) && ((EnergyArray[wFrameCount]<EnergyThreshUsed + 0.5) || (fRatioFreq[wFrameCount]<-4.5)))
		{
			if(EnergyArray[wFrameCount] < (EnergyThreshUsed))	//Energy_Th * 6
				Wait_Duration++;
			else
				Wait_Duration = 0;
			if(fRatioFreq[wFrameCount] < -5.1)
				Wait_Duration1++;
			else
				Wait_Duration1 = 0;
			if ((Wait_Duration > 2) || (Wait_Duration1 >2))
				break;
		}
		else
		{
			Wait_Duration = 0;
			Wait_Duration1 = 0;
		}
	}
	nEndFrame = wFrameCount - 2;

	//寻找起始帧, 从粗判的起始帧往前找
	Wait_Duration = 0;
	Wait_Duration1 = 0;
	for (wFrameCount = nCrudeStart; wFrameCount >= 0; wFrameCount--)
	{
		if((nZeroPassArray[wFrameCount] < 100) && ((EnergyArray[wFrameCount]<EnergyThreshUsed + 0.5) || (fRatioFreq[wFrameCount]<-4.5)))
		{
			if(EnergyArray[wFrameCount] < (EnergyThreshUsed))	//Energy_Th * 6
				Wait_Duration++;
			else
				Wait_Duration = 0;
			if(fRatioFreq[wFrameCount] < -5.0)
				Wait_Duration1++;
			else
				Wait_Duration1 = 0;
			if ((Wait_Duration > 2) || (Wait_Duration1 >2))
				break;
		}
		else
		{
			Wait_Duration = 0;
			Wait_Duration1 = 0;
		}
	}
	nStartFrame = wFrameCount + 2;

	if (nStartFrame < 4)              
		nStartFrame = 4;
	if (nEndFrame > Frame_Num - 4)
		nEndFrame = Frame_Num - 4;

	//如果有声段太短, 则端点检测失败, 将整个语音交给下一级
	if ((nEndFrame - nStartFrame) < 5)        
	{
		nEndFrame = Frame_Num - 4;
		nStartFrame = 4;
	}
}

