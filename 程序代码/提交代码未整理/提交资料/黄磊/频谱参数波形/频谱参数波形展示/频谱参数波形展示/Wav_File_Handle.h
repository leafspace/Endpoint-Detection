#pragma once
#include "Wav_File_Header.h"
#include <cmath>
class Wav_File_Work :protected Wav_File
{
private:

	double *dataDouble;                                                      //新转换的Double型数据，控制范围在[-1,1]

	double *Nor_dataDouble;													 //用于保存归一化数据
	
	double **SERmi;															 //用于保存子带能量比
	double **SFmi;															 //用于保存子带谱流量
	double *RPm;															 //用于保存频谱滚降点
	
	
	


	double *Xn;																 //用于保存短时傅立叶变换后数据
	double *dataDFT;                                                         //用于保存正向离散傅立叶变换数据
	double *dataIDFT;														 //用于保存逆向离散傅立叶变换数据
	double *New_dataDouble;													 //用于保存快速傅立叶变换之前补零后数据
	double *dataFFT;														 //用于保存快速傅立叶变换后数据

	double *Em;															     //用来保存频谱能量
	double *SCm;														     //用来保存频谱质心
	double *BWm;															 //用于保存宽带

	double **Ai;															 //线性预测系数
	double *err;															 //增益
	double *H;																 //线性预测谱
	double **h;																 //线性预测倒谱系数
	double *hSpectrum;										                 //线性预测倒谱


	
	int XnNumber;//存放短时傅立叶变换数据个数
	int dataDFTNumber;//存放正向离散傅立叶变换数据个数
	int dataIDFTNumber;//存放逆向离散傅立叶变换数据个数
	int dataFFTNumber;//存放快速傅立叶变换数据个数
	int EmNumber;//存放频谱能量数据个数
	int SCmNumber;//存放频谱质心数据个数
	int BWmNumber;//存放宽带数据个数
	int AiNumber;//存放线性预测系数个数
	int HNumber;//存放线性预测谱个数
	int hNumber;//存放线性预测倒谱系数个数
	int hSpectrumNumber;//存放线性预测倒谱个数



	bool Conversion_Double(void);                                            //用来将新字节数据转换为Double数据
	double Hamming_window(double data);                                      //汉明窗函数
	int STFT(void);															 //短时傅立叶变换
	bool DFT(void);                                                          //正向离散傅立叶变换，无虚部
	bool IDFT(double *data, unsigned long dataNumber);                       //逆向离散傅立叶变换，无虚部

	int Is_Pow2(void);														 //判断FFT长度是否为2的整数次方
	long Add_Zero(int i);													 //补零
	void Reverse_dataDouble(double *data, int len);						     //乱序输入数据
	bool FFT(void);															 //快速傅立叶变换

	bool spectralEnergy(int len);											 //计算频谱能量
	bool spectralCentroid(int len);										     //频谱质心
	bool bandWidth(int len);												 //宽带

	bool LPC_Levinson_Durbin(void);											 //线性预测系数
	bool LPC(void);															 //线性预测谱
	bool LPCC_Coefficien(void);												 //线性预测倒谱系数
	bool LPCC(void);                                                          //线性预测倒谱

	

public:
	static const int N;                                                      //表示每个窗的帧长
	static const int FrameShift;                                             //窗函数的帧移
	static const double PI;                                                  //数学圆周率PI
	static const int Sub_band;												 //子带个数3
	static const int P;														 //LPC阶数
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

			dataIDFT = (double *)malloc(sizeof(double)*Wav_File::Get_dataNumber());//为逆向傅立叶变换数据申请空间
			if (dataIDFT == NULL) {                                        //分配空间未成功
				throw invalid_argument("ERROR : Memory failure !");
			}
			if (Conversion_Double() == false) {                              //将新字节数据转换为Double数据
				throw invalid_argument("ERROR : Data change failure !");
			}
			this->STFT();
			this->FFT();
			//this->DFT();
			//this->IDFT(dataIDFT, Wav_File::Get_dataNumber());
			this->spectralEnergy(Get_dataFFTNumber());
			this->spectralCentroid(Get_dataFFTNumber());
			this->bandWidth(Get_dataFFTNumber());
			this->LPC_Levinson_Durbin();
			this->LPC();
			this->LPCC_Coefficien();
			this->LPCC();
		}
		
		catch (invalid_argument &e) {
			MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
			exit(-1);
		}
	}
	~Wav_File_Work() {
		free(dataDouble);
	}

	bool Conversion_New(double *a,int n);									 //频谱参数归一化函数

	double Get_Xn(int n);										//用于获取短时傅立叶变换指定位置数据
	double Get_XnNumber(void);									//用于获取短时傅立叶变换数据个数
	
	double Get_dataDFT(int n);									//用于获取正向离散傅立叶变换指定位置数据
	double Get_dataDFTNumber(void);								//用于获取正向离散傅立叶变换数据个数

	double Get_dataIDFT(int n);									//用于获取逆向离散傅立叶变换指定位置数据
	double Get_dataIDFTNumber(void);							//用于获取逆向离散傅立叶变换数据个数


	double Get_dataFFT(int n);									//用于获取快速傅立叶变换指定位置数据
	double Get_dataFFTNumber(void);								//用于获取快速傅立叶变换数据个数

	double Get_Em(int n);								        //用于获取指定位置频谱能量
	double Get_EmNumber(void);									//用于获取频谱能量个数

	double Get_SCm(int n);										//用于获取指定位置频谱质心
	double Get_SCmNumber(void);									//用于获取频谱质心个数

	double Get_BWm(int n);										//用于获取指定位置宽带
	double Get_BWmNumber(void);									//用于获取宽带个数

	double* Get_Ai(int n);										//用于获取指定位置线性预测系数
	double Get_AiNumber(void);									//用于获取线性预测系数个数

	double Get_H(int n);										//用于获取指定位置线性预测谱
	double Get_HNumber(void);									//用于获取线性预测谱个数

	double* Get_h(int n);										//用于获取指定位置线性预测倒谱系数
	double Get_hNumber(void);									//用于获取线性预测谱个数

	double Get_hSpectrum(int n);								//用于获取指定位置线性预测倒谱
	double Get_hSpectrumNumber(void);							//用于获取线性预测倒谱个数
	
	
	
	
	
	int Subband_Energy_Ratio(int len);										 //子带能量比
	void Normalized_Spectral_Signal(int k);								     //归一化频谱信号
	int Subband_Spectral_Flux(int len);									     //子带谱流量
	int Spectral_Rolloff_Point(int len);								     //频谱滚降点
	
	
	
	void ShowData(void);                                                     //覆盖父类的展示数据函数
};
