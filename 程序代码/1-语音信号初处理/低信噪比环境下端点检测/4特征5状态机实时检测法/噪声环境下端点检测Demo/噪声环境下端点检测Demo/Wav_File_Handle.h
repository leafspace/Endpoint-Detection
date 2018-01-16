#pragma once
#include "Wav_File_Header.h"
#include <cmath>
#include <bitset>
#include <complex>
#include <fstream>

#define _PARAGRAPH_MUTE 0                                                    //语音准备阶段
#define _PARAGRAPH_INTERIM 1                                                 //语音假设阶段
#define _PARAGRAPH_VOICE 2                                                   //语音进行阶段
#define _PARAGRAPH_BURST 3                                                   //语音爆破阶段

struct FeatureParameter
{
	double voice;
	double engChart;
	double cepsDistance;
	double engDevia;
	double spectrumEnt;

	FeatureParameter(double voice, double engChart, double cepsDistance,
		double engDevia, double spectrumEnt) {
		this->voice = voice;
		this->engChart = engChart;
		this->cepsDistance = cepsDistance;
		this->engDevia = engDevia;
		this->spectrumEnt = spectrumEnt;
	}
};


struct KalmanFilter
{
	double x;                                                                //系统的状态量
	double p;                                                                //估计误差协方差
	double gain;                                                             //卡尔曼增益
	double A;                                                                //x(n)=A*x(n-1)+u(n),u(n)~N(0,q)
	double H;                                                                //z(n)=H*x(n)+w(n),w(n)~N(0,r)
	double q;                                                                //预测过程噪声协方差
	double r;                                                                //测量过程噪声协方差

	static double predict_q;
	static double newMeasured_q;

	KalmanFilter(double init_x, double init_p) {
		this->x = init_x;                                                    //待测量的初始值，如有中值一般设成中值
		this->p = init_p;                                                    //后验状态估计值误差的方差的初始值（不要为0问题不大）
		this->A = 1;
		this->H = 1;
		this->q = predict_q;                                                 //预测（过程）噪声方差 影响收敛速率，可以根据实际需求给出
		this->r = newMeasured_q;                                             //测量（观测）噪声方差 可以通过实验手段获得
	}
	double kalmanFilter_filter(double newMeasured) {
		this->x = this->A * this->x;                                         //%x的先验估计由上一个时间点的后验估计值和输入信息给出
		this->p = this->A * this->A * this->p + this->q;                     //计算先验均方差 p(n|n-1)=A^2*p(n-1|n-1)+q

		this->gain = this->p * this->H / (this->p * this->H * this->H + this->r);
		this->x = this->x + this->gain * (newMeasured - this->H * this->x);  //利用残余的信息改善对x(t)的估计，给出后验估计，这个值也就是输出
		this->p = (1 - this->gain * this->H) * this->p;                      //%计算后验均方差
		return this->x;
	}
};


class Wav_File_Work :protected Wav_File
{
private:
	double *dataDouble;                                                      //新转换的Double型数据，控制范围在[-1,1]
	double **dataFrameWindow;                                                //保存分帧加窗后的数据
	unsigned long frameNumber;                                               //保存语音帧的个数

	unsigned long voiceNumber;                                               //语音段落个数
	vector<VoiceParagraph> voiceParagraph;                                   //保存这个语音文件中所有要处理的语音段落

	unsigned long fftNumber;                                                 //傅里叶变换后的数据的个数

	double **dataCeps;                                                       //用于保存倒谱数据
	int *dataPitch;                                                          //用于保存在计算完倒谱后的基音周期
	double **dataMfcc;                                                       //用来保存mel倒谱系数
	double *dataCepsDistance;                                                //用来保存倒谱距离
	double *dataEngChart;                                                    //用来保存短时能量
	double *dataEngDevia;                                                    //用来保存能量谱方差
	double **dataAutoFun;                                                    //用来保存自适应函数数据
	double **dataPSDF;                                                       //用来保存功率谱密度函数
	double *dataSpectrumEnt;                                                 //用来保存功率谱谱熵值

	bool Conversion_Double(void);                                            //用来将新字节数据转换为Double数据
	double Hamming_window(double data);                                      //汉明窗函数
	bool Frame_Window(void);                                                 //用来将新字节数据分帧加窗

	bool kalmanFilter_filter(void);                                          //用于将数据进行卡尔曼滤波

	double *DistributionSpace(unsigned long col);                            //分配一维数组空间
	double **DistributionSpace(unsigned long row, unsigned long col);        //分配二维数组空间
	void ReleaseSpace(double **data, unsigned long row);                     //释放二维数组空间

	void Rader(double *data, unsigned long dataNumber);                      //雷德算法，其中dataNumber一定是2^n
	bool DFT(double *data, unsigned long dataNumber);                        //正向离散傅立叶变换，无虚部
	bool IDFT(double *data, unsigned long dataNumber);                       //逆向离散傅立叶变换，无虚部
	double* FFT(double *data, unsigned long dataNumber);                     //快速离散傅立叶变换，无虚部 (严重警告，因为可能出现扩展内存的情况，所以必须返回新地址)
	bool IFFT(double *data, unsigned long dataNumber);                       //快速离散逆傅立叶变换，无虚部
	bool FrameFFT(double** data, unsigned long frameNumber);                 //用于计算分帧后的整体FFT
	bool FrameIFFT(double** data, unsigned long frameNumber);                //用于计算分帧后的整体IFFT
	double Mel_Filter(unsigned long data, int filterFlag);                   //mel 三角滤波器
	double FilterFunction(int flag);                                         //用于计算三角滤波器频率的辅助函数
	bool AutocorrelativeFunction(void);                                      //计算每一帧的自相关函数，是求功率谱谱熵的其中一部分
	bool PSDF(void);                                                         //计算归一化的功率谱密度函数,在运行之前必须运行AutoCorrelativeFunction

	bool NormallizationFun(double *data, unsigned long dataNumber);
	void Normalization(void);                                                //将重点数据归一化操作

	//五状态机操作 (输入为当前帧位置，返回的将是状态的转换情况)

	void FrameVoice(double *voice);                                          //按照一定的计算方式计算每一帧的标准值

	int Mute_State_Process(FeatureParameter framePoint);                     //无语音状态处理
	int Interim_State_Process(FeatureParameter framePoint, unsigned long length);      //语音假设状态处理
	int Voice_State_Process(FeatureParameter framePoint);                    //语音状态处理
	int Burst_State_Process(FeatureParameter framePoint, unsigned long length);        //爆破音/无语音状态处理

	void ShowEndpoint();                                                     //使用这个函数获取所有的段落数据显示
public:
	static const unsigned long N;                                            //表示每个窗的帧长
	static const int FrameShift;                                             //窗函数的帧移
	static const double PI;                                                  //数学圆周率PI
	static const int FilterNumber;                                           //mel 滤波器个数
	static const int CoefficientOrder;                                       //mfcc阶数

	static double threshold_h_EngChart;                                      //用来设置语音信号短时能量高门限
	static double threshold_l_EngChart;                                      //用来设置语音信号短时能量低门限

	static double threshold_h_CepsDistance;                                  //用来设置语音信号倒谱距离高门限
	static double threshold_l_CepsDistance;                                  //用来设置语音信号倒谱距离低门限

	static double threshold_h_EngDevia;                                      //用来设置语音信号能量方差高门限
	static double threshold_l_EngDevia;                                      //用来设置语音信号倒谱距离低门限

	static double threshold_h_SpectrumEnt;                                   //用来设置语音信号能量-熵高门限
	static double threshold_l_SpectrumEnt;                                   //用来设置语音信号能量-熵低门限

	static unsigned long interim_StateLength;                                //语音假设状态最短持续长度
	static unsigned long burst_StateLength;                                  //爆破音\无语音状态最短持续长度

	Wav_File_Work(void) {}
	Wav_File_Work(FILE *f) :Wav_File(f) {
		try
		{
			if (Wav_File::Read_File() == false) {                            //调用父函数读取文件
				throw;
			}

			dataDouble = NULL;
			dataFrameWindow = NULL;
			dataCeps = NULL;
			dataPitch = NULL;
			dataMfcc = NULL;
			dataCepsDistance = NULL;
			dataEngChart = NULL;
			dataEngDevia = NULL;
			dataAutoFun = NULL;
			dataPSDF = NULL;
			dataSpectrumEnt = NULL;

			dataDouble = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber());   //为Double型数据申请内存空间
			if (dataDouble == NULL) {                                        //分配空间未成功
				throw invalid_argument("ERROR : Memory failure !");
			}
			if (Conversion_Double() == false) {                              //将新字节数据转换为Double数据
				throw invalid_argument("ERROR : Data change failure !");
			}

			if (kalmanFilter_filter() == false) {                            //将新字节数据转换为Double数据
				throw invalid_argument("ERROR : Data kalman filter failure !");
			}

			if (Frame_Window() == false) {                                   //分帧加窗
				throw invalid_argument("ERROR : Frame windows failure !");
			}


			//this->Cepstrum();                                                //计算倒频谱
			//this->Pitch();                                                   //计算基音周期

			this->MFCC();                                                    //计算梅尔倒谱系数
			this->CepstrumDistance();                                        //计算倒谱距离


			this->EnergySpectrum();                                          //计算能量谱
			this->EnergyDeviation();                                         //计算能量谱方差

			this->SpectrumEntropy();                                         //计算谱熵

			this->Normalization();

			this->ShowData();

			//this->Endpoint_Detection();
			//this->ShowEndpoint();

		}
		catch (invalid_argument &e) {
			MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
			exit(-1);
		}
	}
	~Wav_File_Work() {
		free(dataDouble);
	}
	double Get_dataNumber(void);                                             //获取Double数据的个数
	double Get_DataDouble(unsigned long Number);                             //获取转换后的Double数据
	int Get_WindowLength(void);                                              //获取帧长（窗的大小）
	unsigned long Get_FrameNumber(void);                                     //获取帧数

	double Get_DataCepsDistance(unsigned long Number);                       //获取指定位置上的倒谱距离
	double Get_DataEngChart(unsigned long Number);                           //获取指定位置上的能量谱
	double Get_DataEngDevia(unsigned long Number);                           //获取指定位置上的能量谱方差
	double Get_DataSpectrumEnt(unsigned long Number);                        //获取指定位置上的功率谱谱熵

	unsigned long Get_voiceNumber(void);                                     //获取语音段落个数
	VoiceParagraph Get_dataVoicePoint(unsigned long Number);                 //获取某个语音点
	void ShowData(void);                                                     //覆盖父类的展示数据函数
	void SaveNewWav(char* path);                                             //保存去掉空白处的语音文件
	void SaveParagraphInfo(char* path);                                      //保存段落信息
	bool Cepstrum(void);                                                     //倒频谱
	bool Pitch(void);                                                        //基音周期（严重注意-运行前必须先计算倒谱Cepstrum）
	bool MFCC(void);                                                         //梅尔倒谱系数
	bool CepstrumDistance(void);                                             //计算倒谱距离（严重注意-运行前必须先计算mel倒谱系数MFCC）
	bool EnergySpectrum(void);                                               //计算短时能量
	bool EnergyDeviation(void);                                              //计算能量谱方差(严重注意-运行谦必须先计算能量谱)
	bool SpectrumEntropy(void);                                              //计算谱熵
	bool Endpoint_Detection(void);                                           //端点检测函数
};
