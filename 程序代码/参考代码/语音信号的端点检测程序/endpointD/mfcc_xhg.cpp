/*******************************************************************
****	文件：		mfcc_xhg.cpp								****
****	功能：		计算MFCC所用到的一些基本函数				****
**** 			 浮点程序										****
****	最后定型01.08.20, 许海国		          				****
//MFCC特征提取实现的主程序，其它另外需要的说明均在
//MFCC_xhg.h中, 程序中只需要包含MFCC_xhg.h即可
*******************************************************************/


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "cst_lib.h"
#include "mfcc_xhg.h"
#include "basop.h"
#include "endPoint_xhg.h"
#include "16to8.h"

extern FSDEF fs;

/*=====================================================
函数名称：	StartEnd
函数功能：	对一句话的语音数据进行端点检测 
函数性质：	
输入参数：	*sData 一句话的首地址
			lDataLength	一句话的长度（样点个数）
			*lStartPnt	返回起始样点
			*lEndPnt	返回结束样点
			lSampleRate	采样率
输出参数：	
备注：		
基本思路：
====================================================*/
void StartEnd(short *sData, long lDataLength, 
			  long *lStartPnt, long *lEndPnt,
			  long lSampleRate)
{
	long	FrameTotalNumber;//一句话中的帧计数器
	int	lDataLength_8000, i;
	short	*sdata_8000=NULL;
	short	nStartFrame, nEndFrame;
	int OptionMode=0;
	    
    
		lDataLength_8000=lDataLength;
		sdata_8000=sData;
	FrameTotalNumber=(long)((sizeof(short)*lDataLength_8000- (sizeof(short)*FRAME_LEN))/(sizeof(short)*FRAME_STEP)) + 1;

	OptionMode = OptionMode | MFCC | PREEMPHASIS  ;
	Comp_Feature(OptionMode, sdata_8000, FrameTotalNumber, nStartFrame, nEndFrame);
//-------------------------------------------------------------	
	for(i = 0; i<lDataLength-1; i++)      //added by xiaogang 2003.9.2
	{
        if ( (sdata_8000[i+1]-sdata_8000[i]) == 1)
		{
			(*lStartPnt) = i/2;
		    break;
		}
        (*lStartPnt) = 0;
	}
	for(i=0 ; i<lDataLength-1; i++)
	{
        if ( (sdata_8000[i]-sdata_8000[i+1]) == 1)
		{
			(*lEndPnt) = i/2;
		    break;
		}
		(*lEndPnt) = 0;
	}
   
	if(((*lStartPnt)>=(*lEndPnt)) || (((*lEndPnt)-(*lStartPnt)) < 5)  )
	{
	     (*lStartPnt)=0;
         (*lEndPnt  )=0;
         //getchar();
	}

//--------------------------------------------------------------	

//	(*lStartPnt)=(long)((nStartFrame-2)*FRAME_STEP);
//	(*lEndPnt)=(long)((nEndFrame+6)*FRAME_STEP);

	if( ((*lStartPnt)>lDataLength_8000) || ((*lStartPnt)<0) )
		(*lStartPnt)=0;

	if( ((*lEndPnt)>lDataLength_8000) || ((*lEndPnt)<0) )
		(*lEndPnt)=lDataLength_8000;
/*   针对8k seg文件时才用          
	if(lSampleRate==16)
	{
		(*lStartPnt)=(*lStartPnt)*2;
		(*lEndPnt)=(*lEndPnt)*2;
		sdata_8000=NULL;
	}
*/
}

/*=====================================================
函数名称：	Comp_Feature
函数功能：	对一句话的语音数据进行处理, 求出它们的特征, 包括倒谱(可选择的特征, 差分倒谱, 
			能量, 差分能量, 二阶差分能量, 并判断出端点, 起始帧号 & 结束帧号
函数性质：	
输入参数：	OptionMode		模式参数，MFCC or LPCC, CMS, ...
			sData			语音数据区的头，建议采用连续数据区的数据指针
			Frame_Number	一句话中, 总的语音帧数
输出参数：	FeatureArray	对应于该帧语音的特征矢量，已分配了内存
			nStartFrame		起始帧
			nEndFrame		结束帧
备注：		
基本思路：
====================================================*/
void Comp_Feature(int OptionMode, short *sData, int FrameNumber, short &nStartFrame, short &nEndFrame)
{
	short	wSampleCount, wFrameCount;			//样点和帧的循环计数变量
	short	*tmpVoiceHead;						//用来指向每一个语音帧的头, 地址是会改变的, 但是数据是不变的
	float	fOneFrameData[FRAME_LEN];			//一帧语音数据, 其数据是会在计算过程中改变

	static	int		first =1;					//标志位, hamming窗, cepweightwindow, 三角窗
	static	float	HanmingWindow[FRAME_LEN];	//hanming窗函数

	//用于端点检测
	float	*EnergyArray		= NULL;			//用来保存每一帧的能量
	short	*nZeroPassArray		= NULL;			//用来保存每一帧的过零率	
	float	*fRatioFreq			= NULL;			//用来保存每一帧的低频能量
	float	DcLevelOneSentence	= 0;
	static  float	EnergyTh;					//用于端点检测的能量门限

	//动态分配对数能量的内存
	EnergyArray		= new float [FrameNumber];
	//动态分配过零率的内存
	nZeroPassArray	= new short [FrameNumber];
	//动态分配低频能量的内存
	fRatioFreq		= new float [FrameNumber];

	//获取语音数据存储地址
	tmpVoiceHead = sData;
	
	//判断如果是第一次, 这初始化hanming窗, 倒谱加权窗, (MFCC)Mel三角窗
	if(first)
	{
		first = 0;
		Initial(OptionMode, HanmingWindow);
	}

	//---------------->
	//对一句话中的每一帧进行循环, 求出其倒谱特征
	for(wFrameCount=0; wFrameCount<FrameNumber; wFrameCount++)
	{
	
		//从语音数据缓冲区取得一帧的语音数据进行计算
		for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
		{
			fOneFrameData[wSampleCount] = tmpVoiceHead[wSampleCount];
			DcLevelOneSentence += fOneFrameData[wSampleCount];
		}

		//2.预加重		y[n] = x[n] - preemphasis_factor * x[n-1]
		if( OptionMode & PREEMPHASIS)
			PreEmphasis(fOneFrameData);

		//4.加hanming窗											n
		//		HanmingWindow[n] = 0.54 - 0.46 * cos( 2 * PI *------);	N:帧长
		//													  N - 1
		for(wSampleCount=0; wSampleCount<FRAME_LEN; wSampleCount++)
			fOneFrameData[wSampleCount] = fOneFrameData[wSampleCount] * HanmingWindow[wSampleCount];

		//5.计算一帧的MFCC特征, 包括低频能量
		Comp_Mfcc(fOneFrameData, fRatioFreq[wFrameCount]);

		//7.进行循环的处理
		//将tmpVoiceHead指向下一帧语音的起始处
		tmpVoiceHead	+= FRAME_STEP;

	}//<---------------------
	//此处一句话的语音循环结束, 计算完一句话的倒谱

	//此处进行一句话的能量计算
	tmpVoiceHead	= sData;

	DcLevelOneSentence /= (FrameNumber * FRAME_LEN);

	//计算基本的能量,  过零率
	EnergyCompute(tmpVoiceHead, EnergyArray, nZeroPassArray, FrameNumber, DcLevelOneSentence);

	//计算用于端点检测的能量门限
	EnergyThreshCompte(EnergyArray, FrameNumber, EnergyTh);

/*--------------------------------------->
**			以后都是为了端点检测		***
*****************************************/
	//对低频能量进行归一化, 把所有的低频能量减去其最大值
	//找出最大值, 由于总的能量的动态范围太大, 故采用这种归一化的方法并不是很合适的.
	double	tmpRatio = -1e50;
	for(wFrameCount=0; wFrameCount<FrameNumber; wFrameCount++)
		if(tmpRatio < fRatioFreq[wFrameCount])
			tmpRatio = fRatioFreq[wFrameCount];

	//归一化
	for(wFrameCount=0; wFrameCount<FrameNumber; wFrameCount++)
		fRatioFreq[wFrameCount] -= (float)tmpRatio;

	//端点检测
	//现在需要输入的参数有: 能量门限, 每一帧的能量 & 过零率 & 低频能量,
	EndPointDetection(EnergyArray, nZeroPassArray, fRatioFreq, FrameNumber, nStartFrame, nEndFrame, EnergyTh);
	//删除内存的分配
	if(EnergyArray!=NULL)
	{
		delete EnergyArray;
	    EnergyArray=NULL;
	}
    if(nZeroPassArray!=NULL)
	{
		delete nZeroPassArray;
	    nZeroPassArray=NULL;
	}
	if(fRatioFreq!= NULL)
	{
		delete fRatioFreq;
		fRatioFreq = NULL;
	}

}

/*=====================================================
函数名称：	Comp_Mfcc
函数功能：	求取单帧语音的CEP_NUM维MFCC系数同时放在特征矢量的前CEP_NUM维, 
			求的低频能量
输入参数：	fOneFrameData				单帧语音数据区的头，建议采用连续数据区的数据指针
输出参数：	fAreaEnergy					低频频域的能量, 频率段可以在cst_lib.h中设置
备注：		
基本思路：
====================================================*/
void	Comp_Mfcc(float *fOneFrameData,	float &fAreaEnergy)//!!!
{
	short	wFftCount;					//用来作为循环中的计数变量
	float	fFFTAm[MAX_FREQ_POINT];		//保存FFT频谱的幅度	
	COMPLEX cFFTData[FFT_NUM];			//用来保存复数的语音数据

	//对用于FFT的cFFTData 进行初始化
	for(wFftCount=0; wFftCount<FFT_NUM; wFftCount++)
	{
		cFFTData[wFftCount].real = 0.0;
		cFFTData[wFftCount].image = 0.0;
	}
	for(wFftCount=0; wFftCount<FRAME_LEN; wFftCount++)
		cFFTData[wFftCount].real = fOneFrameData[wFftCount];

	//1.计算FFT, 获得语音信号的频谱
	FFT(cFFTData, FFT_ORDER);

	//2.由FFT的计算结果, 求得信号的幅度谱
	//			FFTAm(x, y) = x^2 + y^2
	FFTAm(cFFTData, fFFTAm, FFT_ORDER);

	//计算第100hz-400hz频域的能量, 因为浊音在该频域的能量比较高,
	AreaFreqComp(fFFTAm, fAreaEnergy);
}


/*=====================================================
函数名称：FFT
函数功能：FFT变换，基2DIT
输入参数: x 复数点序列,
		  m FFT的级数
输出参数：无,原址操作
====================================================*/
void FFT(COMPLEX *input, int x)
{
	int n , i , nv2 , j , k , le , l , le1 , ip , nm1 ;
	COMPLEX t , u , w ;

	n = 1;
	for(i=0; i<x; i++)
		n = n*2 ;

	nv2 = n / 2 ;
	nm1 = n - 1 ;
  	j = 1 ;

	for (i = 1 ; i <= nm1 ; i ++)
	{
		if (i < j)
		{
			t.real = input[i - 1].real ;
			t.image = input[i - 1].image ;
			input[i - 1].real = input[j - 1].real ;
			input[i - 1].image = input[j - 1].image ;
			input[j - 1].real = t.real ;
			input[j - 1].image = t.image ;
		}

		k = nv2 ;

		while (k < j)
		{
			j -= k ;
			k /= 2 ;
		}
		j += k ;
	}

	le = 1 ;
	for (l= 1 ; l <= x ; l ++)
	{
		le *= 2 ;
   		le1 = le / 2 ;
		u.real = 1.0f ;
		u.image = 0.0f ;
		w.real = (float) cos(PI / le1) ;
		w.image =(float) -sin(PI / le1) ;

		for (j = 1 ; j <= le1 ; j ++)
		{
			for (i = j ; i <= n ; i += le)
			{
				ip = i + le1 ;
				t.real = input[ip - 1].real * u.real - input[ip - 1].image * u.image ;
				t.image = input[ip - 1].real * u.image + input[ip - 1].image * u.real ;
				input[ip - 1].real = input[i - 1].real - t.real ;
				input[ip - 1].image = input[i - 1].image - t.image ;
				input[i - 1].real = t.real + input[i - 1].real ;
				input[i - 1].image = t.image + input[i - 1].image ;
			}

			t.real = u.real * w.real - u.image * w.image ;
			t.image = u.image * w.real + u.real * w.image ;
			u.real = t.real ;
      		u.image = t.image ;
		}
	}
}

/*=====================================================
函数名称：	FFTAm
函数功能：	求取FFT变换后的幅度 s(i) = x(i)^ 2 + y(i)^ 2
输入参数：	x 原始FFT复数点 ,
			y 输出的存储FFT幅度的序列，由于对称所以只需算一半,
			m FFT的级数 
返回参数：	无 
====================================================*/
void FFTAm(COMPLEX *x , float *y, long m)
{
	long n,i;
	n = 1 ;
	for (i = 0 ; i < m ; i ++)
		n *= 2 ;
	for (i = 0 ; i < n/2 ; i ++)
      y[i] =(float) (x[i].real * x[i].real + x[i].image * x[i].image) ;
}

/*=====================================================
函数名称：	AreaFreqComp
函数功能：	计算区段频域的能量								
输入参数：	fFFTAm[MAX_FREQ_POINT]		FFT能量序列 	
输出参数：	fAreaEnergy					区段频率的能量										
====================================================*/
void	AreaFreqComp(float fFFTAm[MAX_FREQ_POINT], float &fAreaEnergy)
{
	short wFFTCount;
	
	//计算出与频率相对应的频点
	static short	lowFreq = (short) (LOW_FREQ_AREA / SAMPLE_FREQUENCY * FFT_NUM);
	static short	highFreq = (short) (HIGH_FREQ_AREA / SAMPLE_FREQUENCY * FFT_NUM);

	fAreaEnergy = 0;
	for(wFFTCount=lowFreq; wFFTCount<highFreq; wFFTCount++)
		fAreaEnergy += fFFTAm[wFFTCount];
	fAreaEnergy = (float)log(fAreaEnergy);
}
