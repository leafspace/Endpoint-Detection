//***宏定义

//一些常数的定义
#define		PI					3.14159265358979f
#define		M_PI				PI
#define		TPI					6.28318530717959     /* PI*2 */

//一些与语音原始数据有关的定义
#define		SAMPLE_FREQUENCY	8000.0					//采样率HZ   
#define		FRAME_LEN			256						//每帧的采样点数
#define		OVERLAP_NUM			FRAME_LEN/2				//帧间重叠的采样点数		
#define		FRAME_STEP			(FRAME_LEN-OVERLAP_NUM)	//帧移的点数
#define     LPCCFIX_FRAME		192
//一些与语音预处理有关的定义
#define		PRE_EMPHASIS_FACTOR	0.98f	//预加重系数
#define		ZEROPASS_NOISE_FACT	1.5f	//噪声能量的自适应
#define		ADAPTIVE_FACTOR		0.8f	//门限自适应的加权系数
//端点检测
#define		LOW_FREQ_AREA		100						//计算频域能量时取的低频
#define		HIGH_FREQ_AREA		600					//计算频域能量时取的高频

//一些与特征有关的定义
/****************************
MFCC所用的定义
****************************/
#define		FFT_ORDER			8						//FFT变换级数
#define		FFT_NUM				256						//FFT变换点数
#define		MAX_FREQ_POINT		(FFT_NUM / 2)

//工作模式的定义
#define		MFCC				0x0001				//使用MFCC特征
#define		LPCC				0x0002				//使用LPCC特征

#define		VAD					0x0004              //VAD 判决  
#define		CMS					0x0008              //倒谱均值减
#define		PREEMPHASIS			0x0010
#define		HIGHPASS			0x0020				//是否使用高通
#define		DELTFEATURE			0x0040				//使用一阶差分特征
#define		ENERGY				0x0080
#define		DELTENERGY			0x0100				//使用一阶差分能量
#define		DDELTENERGY			0x0200				//使用二阶差分能量
