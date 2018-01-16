/*******************************************************************
****	文件：		basop.h					****
****	功能：		计算LPCC和MFCC所用到的一些共同的基本函数		****
**** 			                             		****
****	修改记录：	2001.7.23 摘自G.732.1              	****
*******************************************************************/



//	进行初始化
	void	Initial(int OptionMode, float	HanmingWindow[FRAME_LEN]);
//  预加重 
	void	PreEmphasis(float *fOneFrameData);
//计算能量, 过零率, 能量门限
	void	EnergyCompute(short *tmpVoiceHead, float *EnergyArray, short *nZeroPassArray, short FrameNumber, float DcLevelOneSentence);
//计算能量门限, 不过是在所以帧的能量都已经计算完毕
	void	EnergyThreshCompte(float *EnergyArray, short FrameNumber, float &EnergyTh);

//****基本计算
//	符号计算
	int		sign(float x);
