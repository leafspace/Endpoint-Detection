/*******************************************************************
****	文件：		basop.cpp									****
****	功能：		计算LPCC 和 MFCC所用到的一些共同的基本函数	****
**** 			 浮点程序										****
****	最后定型01.08.20, 许海国		          				****
*******************************************************************/

#include  <math.h>

#include "cst_lib.h"
#include "basop.h"
#include "mfcc_xhg.h"

/*=====================================================
函数名称：	Initial
函数功能：	对Hanming窗系数
函数性质：	
输入参数：	OptionMode	工作模式选择, LPCC或者是MFCC, 或者其它
输出参数：	HanmingWindow[FRAME_LEN]	//hanming窗函数
备注:		每次程序运行, 只需要一次初始化就可以了,
			本函数使用到的常数有 
			FRAME_LEN			帧长 
====================================================*/
void	Initial(int OptionMode, float	HanmingWindow[FRAME_LEN])
{
	int	wSampleCount;		//循环用变量, 样点, 倒谱
	float fDataTemp;					//中间变量
	
	//初始化hanming窗函数
	//HANMING 0.54 - 0.46 * COS( 2 * n * PI / ( N - 1) ) (N为帧长)
	fDataTemp = (float) 2.0 * PI / (FRAME_LEN - 1);
	for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
		HanmingWindow[wSampleCount] = (float) (0.54 - 0.46 * cos( wSampleCount * fDataTemp));

}

/*=====================================================
函数名称：	PreEmphasis
函数功能：	对原始语音数据预加重
输入参数：	fOneFrameData 单帧语音数据内存头 
输出参数:	无
备注：		每帧的前一个点取第1个点的值代替  S'(n)=S(n)-0.98*S(n-1)
			用到的常数定义有
			FRAME_LEN			帧长
			PRE_EMPHASIS_FACTOR	预加重系数
====================================================*/
void PreEmphasis(float *fOneFrameData)
{
	
    int   i;
    float acc0[FRAME_LEN];	
    
   	acc0[0] = fOneFrameData[0] - PRE_EMPHASIS_FACTOR * fOneFrameData[0];
	for (i=1; i < FRAME_LEN; i++)
       	acc0[i] = fOneFrameData[i] -PRE_EMPHASIS_FACTOR * fOneFrameData[i-1];
        
	for (i=0; i < FRAME_LEN; i++)
   	 	fOneFrameData[i] = acc0[i];
}

/*=====================================================
函数名称：	sign
函数功能:	符号计算
输入参数:	x	需要计算符号的数据
输出参数:	该数据的符号位, x>0, 符号为1, x=<0, 符号位为-1
====================================================*/
int sign(float x)
{
  if(x > 0)
    return 1;
  else
    return -1;
}

/*=====================================================
函数名称：	EnergyThreshCompte
函数功能:	能量门限计算
输入参数:	EnergyArray	一句话的能量序列
			FrameNumber 一句话的语音帧数
输出参数:	EnergyTh	一句话的能量门限
====================================================*/
void	EnergyThreshCompte(float *EnergyArray, short FrameNumber, float &EnergyTh)
{
	float EnergyOneSentence	= 0;
	float EnergyFront		= 0;
	float EnergyEnd			= 0;

	static short first = 1;
//	short first = 1;
	
	//前2帧的能量进行平均
	//如果相邻2帧的能量差异太大, 则取最小值
	if(fabs(EnergyArray[0] - EnergyArray[1]) < 1.0 )
		EnergyFront = (EnergyArray[0] + EnergyArray[1]) / 2;
	else
	{
		if(EnergyArray[0] < EnergyArray[1])
			EnergyFront = EnergyArray[0];
		else
			EnergyFront = EnergyArray[1];
	}
	
	//后2帧的能量进行平均
	if(fabs(EnergyArray[FrameNumber-2] - EnergyArray[FrameNumber-1]) < 1.0)
		EnergyEnd = (EnergyArray[FrameNumber-2] + EnergyArray[FrameNumber-1]) / 2 ;
	else
	{
		if(EnergyArray[FrameNumber-2] < EnergyArray[FrameNumber-1])
			EnergyEnd = EnergyArray[FrameNumber-2];
		else
			EnergyEnd = EnergyArray[FrameNumber-1];
	}

	//前面的能量和后面的能量进行平均
	if(fabs(EnergyFront - EnergyEnd) < 2.0)
		EnergyOneSentence = (EnergyFront + EnergyEnd) / 2;
	else
	{
		if(EnergyFront < EnergyEnd)
			EnergyOneSentence = EnergyFront;
		else
			EnergyOneSentence = EnergyEnd;
	}

	if(first == 1)
	{
		EnergyTh = EnergyOneSentence ;
		first = 0;
	}
	else
		EnergyTh =(float)( ADAPTIVE_FACTOR * EnergyTh + (1-ADAPTIVE_FACTOR) * EnergyOneSentence);

}

/*=====================================================
函数名称：	EnergyCompute
函数功能:	计算一句话的语音能量, 过零率
输入参数:	tmpVoiceHead			一句话的原始语音
			FrameNumber				一句话中的语音帧数
			DcLevelOneSentence		一句话的直流电平
输出参数：	EnergyArray				该帧语音的能量
			nZeroPassArray			该句话的过零率
说明:		
			常数:			FRAME_LEN
=====================================================*/
void	EnergyCompute(short *tmpVoiceHead, float *EnergyArray, short *nZeroPassArray, short FrameNumber, float DcLevelOneSentence)
{
	short wFrameCount, wSampleCount;
	float fOneFrameData[FRAME_LEN];
	
	float tmpNoise, fTmp, NoiseLevelOneSentence;
	float NoiseFront	= 0;
	float NoiseEnd		= 0;
	float tmpPointLevel[FRAME_LEN];
	float *pNoise;
	static short first = 1;
	static float NoiseLevelAll;


	//计算能量
	for(wFrameCount=0; wFrameCount<FrameNumber; wFrameCount++)
	{
		EnergyArray[wFrameCount] = 0.0 ;
		for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
		{
			fOneFrameData[wSampleCount] = tmpVoiceHead[wSampleCount] - DcLevelOneSentence;
			EnergyArray[wFrameCount] += fOneFrameData[wSampleCount] * fOneFrameData[wSampleCount];
		}
	
		//对能量取对数
		if(EnergyArray[wFrameCount] > 1)
			EnergyArray[wFrameCount] = (float)log(EnergyArray[wFrameCount]);
		else
			EnergyArray[wFrameCount] = 0;
	
		/*------------------------------------------------------->*/
		//				过零率的计算
		//对噪声电平进行自适应
		if((wFrameCount ==0 ) || (wFrameCount==1) || (wFrameCount==FrameNumber-2) || (wFrameCount==FrameNumber-1))
		{
			//将所有点电平的绝对值取和
			tmpNoise = 0;
			for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
				tmpNoise += (float)fabs(fOneFrameData[wSampleCount]);
			tmpNoise /= FRAME_LEN;
			if((wFrameCount == 0) || (wFrameCount == FrameNumber -2))
				NoiseLevelOneSentence = tmpNoise;
			else
			{
				//一句话的噪声电平和噪声能量都是采用前两帧的平均
				if(wFrameCount == 1)
					pNoise = &NoiseFront;
				else
					pNoise = &NoiseEnd;
				fTmp = tmpNoise / NoiseLevelOneSentence;
				if((fTmp<2) && (fTmp>0.5))
					*pNoise = (NoiseLevelOneSentence + tmpNoise) / 2;
				else if(fTmp > 2)
					*pNoise = NoiseLevelOneSentence;
				else
					*pNoise = tmpNoise;
				if(wFrameCount == 1)
				{
					if(first == 1)
						NoiseLevelAll = *pNoise;
				}
				else
				{
					fTmp = NoiseFront / NoiseEnd;
					if((fTmp<2) && (fTmp>0.5))
						NoiseLevelOneSentence = (NoiseFront + NoiseEnd) / 2;
					else if(fTmp > 2)
						NoiseLevelOneSentence = NoiseEnd;
					else
						NoiseLevelOneSentence = NoiseFront;
					//如果是第1句话, 则直接采用这句话的噪声电平和噪声能量
					if(first == 1)
					{
						NoiseLevelAll = NoiseLevelOneSentence;
						first = 0;
					}
					//如果不是, 则采用以前的噪声电平和噪声能量的加权值作为噪声电平和噪声能量
					else
						NoiseLevelAll =(float)( ADAPTIVE_FACTOR * NoiseLevelAll + (1-ADAPTIVE_FACTOR) * NoiseLevelOneSentence);
				}
			}
		}	
		
		//对样点进行滤波, 如果信号的电平小于噪声电平, 则将信号的幅度取为0
		for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
		{
			// 如果信号大于0, 则减去噪声的门限电平, 如果它小于0, 则认为是噪声, 让它取0,
			if(sign(fOneFrameData[wSampleCount]) == 1)
			{
				tmpPointLevel[wSampleCount] = fOneFrameData[wSampleCount] - ZEROPASS_NOISE_FACT * NoiseLevelAll;
				if(sign(tmpPointLevel[wSampleCount]) == -1)
					tmpPointLevel[wSampleCount] = 0;
			}
			//如果信号小于0, 则加上噪声的门限电平, 如果它大于0, 则认为是噪声, 让它取0,
			else
			{
				tmpPointLevel[wSampleCount] = fOneFrameData[wSampleCount] + ZEROPASS_NOISE_FACT * NoiseLevelAll;
				if(sign(tmpPointLevel[wSampleCount]) == 1)
					tmpPointLevel[wSampleCount] = 0;
			}
		}

		nZeroPassArray[wFrameCount] = 0;
		for(wSampleCount=0; wSampleCount<FRAME_LEN-1; wSampleCount++)
			nZeroPassArray[wFrameCount] += (int) fabs(float(sign(tmpPointLevel[wSampleCount +1]) - sign(tmpPointLevel[wSampleCount])));
		nZeroPassArray[wFrameCount] /= 2;

		//tmpVoiceHead指向下一帧的起始处
		tmpVoiceHead	+= FRAME_STEP;
	}
}
