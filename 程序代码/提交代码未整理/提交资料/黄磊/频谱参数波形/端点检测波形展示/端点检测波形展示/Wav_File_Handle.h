#pragma once
#include "Wav_File_Header.h"
#include <cmath>

#define MUTEPARAGRAPH 0                                                      //静音段
#define INTERIMPARAGRAPH 1                                                   //过渡段
#define VOICEPARAGRAPH 2                                                     //语音段
#define OVERPARAGRAPH 3                                                      //语音结束

class Wav_File_Work :protected Wav_File
{
private:
	double energyHigh;                                                       //短时帧能量高门限
	double energyLow;                                                        //短时帧能量低门限
	double zcrHigh;                                                          //短时过零率高门限
	double zcrLow;                                                           //短时过零率低门限

	static const unsigned long minSilence;                                   //最短静音长度
	static const unsigned long minVoiceLength;                               //最短语音长度

	double *dataDouble;                                                      //新转换的Double型数据，控制范围在[-1,1]
	vector<double> dataEnergy;                                               //保存短时帧能量，个数为dataNunber-N
	vector<double> dataZCR;                                                  //保存短时过零率

	double maxEnergy;                                                        //用于保存最大的短时帧能量
	double minEnergy;                                                        //用于保存最小的短时帧能量
	double maxZCR;                                                           //用于保存最大的短时过零率
	double minZCR;                                                           //用于保存最小的短时过零率

	unsigned long voiceNumber;                                               //语音段落个数
	vector<VoiceParagraph> voiceParagraph;                                   //保存这个语音文件中所有要处理的语音段落

	bool Conversion_Double(void);                                            //用来将新字节数据转换为Double数据
	double Hamming_window(double data);                                      //汉明窗函数
	short Sign_Function(double data);                                        //求短时过零率的辅助符号函数
	bool Frame_Energy(void);                                                 //用于求短时帧能量
	bool Frame_ZCR(void);                                                    //用于求短时过零率
	bool Frame_EnergyZcr(void);                                              //用于同时求取短时帧能量与短时过零率
public:
	static const int N;                                                      //表示每个窗的帧长
	static const int FrameShift;                                             //窗函数的帧移
	static const double PI;                                                  //数学圆周率PI
	Wav_File_Work(void) {}
	Wav_File_Work(FILE *f) :Wav_File(f) {
		try
		{
			if (Wav_File::Read_File() == false) {                            //调用父函数读取文件
				throw;
			}
			dataDouble = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber());   //为Double型数据申请内存空间
			if (dataDouble == NULL) {                                        //分配空间未成功
				throw invalid_argument("ERROR : Memory failure !");
			}
			if (Conversion_Double() == false) {                              //将新字节数据转换为Double数据
				throw invalid_argument("ERROR : Data change failure !");
			}
			this->Endpoint_Detection();                                      //开始端点检测
		}
		catch (invalid_argument &e) {
			MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
			exit(-1);
		}
	}
	~Wav_File_Work() {
		free(dataDouble);
	}
	double Get_maxEnergy(void);                                              //获取最大短时帧能量
	double Get_minEnergy(void);                                              //获取最小短时帧能量
	double Get_maxZCR(void);                                                 //获取最大短时过零率
	double Get_minZCR(void);                                                 //获取最小短时过零率
	double Get_dataNumber(void);                                             //获取Double数据的个数
	double Get_dataEZNumber(void);                                           //获取能量过零率的个数
	double Get_DataDouble(unsigned long Number);                             //获取转换后的Double数据
	double Get_DataEnergy(unsigned long Number);                             //依据序号找到对应的短时帧能量
	double Get_DataZCR(unsigned long Number);                                //依据序号找到对应的短时过零率
	int Get_WindowLength(void);                                              //获取帧长（窗的大小）
	unsigned long Get_voiceNumber(void);                                     //获取语音段落个数
	VoiceParagraph Get_dataVoicePoint(unsigned long Number);                 //获取某个语音点
	void ShowData(void);                                                     //覆盖父类的展示数据函数
	void SaveNewWav(void);                                                   //保存去掉空白处的语音文件
	bool Endpoint_Detection(void);                                           //端点检测函数
};
