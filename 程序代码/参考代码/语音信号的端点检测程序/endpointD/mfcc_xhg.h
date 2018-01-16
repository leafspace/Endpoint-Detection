/***************************************************************

功能：		该子程序包含MFCC特征提取的函数
			修改记录：	2001.7.31  许海国 写出基本程序
****************************************************************/

//MFCC的头文件,函数的声明均在此处,
#include "cst_lib.h"

//几个自定义类型的定义
typedef struct
	{
		float real ;
		float image ;
	} COMPLEX ;								//复数结构

void StartEnd(short *sData, long lDataLength, 
			  long *lStartPnt, long *lEndPnt,
			  long lSampleRate);


//函数声明///////////////////////////
//提取一句话的特征
void Comp_Feature(int OptionMode, short *sData, int FrameNumber, short &nStartFrame, short &nEndFrame);


//FFT
void	FFT(COMPLEX *x , int m);		
//FFT的幅度							
void	FFTAm(COMPLEX *x , float *y, long m);		
//一帧的MFCC特征的提取
void	Comp_Mfcc(float *fOneFrameData, float &fAreaEnergy);//!!!
//计算区段频域能量
void	AreaFreqComp(float fFFTAm[MAX_FREQ_POINT], float &fAreaEnergy);






