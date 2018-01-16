#include "stdafx.h"
#include "Wav_File_Handle.h"

double KalmanFilter::predict_q = 0.7;                                        //预测过程中的噪声协方差    （严重注明：此处的数值需要经过历史经验与多次实验才能获得最佳数据）
double KalmanFilter::newMeasured_q = 0.7;                                    //测量过程中的噪声协方差    （严重注明：此处的数值需要经过历史经验与多次实验才能获得最佳数据）

const unsigned long Wav_File_Work::N = 256;                                  //初始化每个窗的窗长
const int Wav_File_Work::FrameShift = 10;                                    //初始化窗函数的帧移
const double Wav_File_Work::PI = 3.14159;                                    //初始化圆周率
const int Wav_File_Work::FilterNumber = 24;                                  //mel 滤波器个数
const int Wav_File_Work::CoefficientOrder = 12;                              //mfcc阶数

double Wav_File_Work::threshold_h_CepsDistance = 0.8;
double Wav_File_Work::threshold_h_EngChart = 0.8;
double Wav_File_Work::threshold_h_EngDevia = 0.8;
double Wav_File_Work::threshold_h_SpectrumEnt = 0.8;

double Wav_File_Work::threshold_l_CepsDistance = 0.2;
double Wav_File_Work::threshold_l_EngChart = 0.2;
double Wav_File_Work::threshold_l_EngDevia = 0.2;
double Wav_File_Work::threshold_l_SpectrumEnt = 0.2;

unsigned long Wav_File_Work::interim_StateLength = 10;                       //语音假设状态最短持续长度
unsigned long Wav_File_Work::burst_StateLength = 5;                          //爆破音\无语音状态最短持续长度

bool Wav_File_Work::Conversion_Double(void)                                  //用来将新字节数据转换为Double数据
{
	try
	{
		cout << "TIP : Change to double data ..." << endl;
		const double Flag = pow((double)2, Wav_File::Get_ChunkAlign() * 8);  //表示的是原数据最大值的一半
		for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {     //遍历每个数据
			dataDouble[i] = (double)(Wav_File::Get_Data(i) / Flag);          //控制每个数据在[-1,1]之间
		}
	}
	catch (invalid_argument &e) {
		cerr << e.what() << endl;
		MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
		return false;
	}
	return true;
}

double Wav_File_Work::Hamming_window(double data)                            //汉明窗函数
{
	if (data >= 0 && data <= (N - 1)) {                                      //0<= n <= N-1的情况
		return 0.54 - 0.46*cos(2 * PI*data / (N - 1));                       //返回数值
	}
	else {
		return 0;
	}
}

bool Wav_File_Work::Frame_Window(void)                                       //用来将新字节数据分帧加窗
{
	cout << "TIP : Frame window ..." << endl;
	if (dataFrameWindow) {                                                   //如果原来有数据
		ReleaseSpace(dataFrameWindow, frameNumber);
	}
	frameNumber = (unsigned long)(Wav_File_Work::Get_dataNumber() - Wav_File_Work::N) / Wav_File_Work::FrameShift + 1;         //求帧数
	if (frameNumber < 1) {
		return false;
	}
	dataFrameWindow = DistributionSpace(frameNumber, Wav_File_Work::N);
	for (unsigned long i = 0; i < frameNumber; ++i) {                        //这是帧的个数
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {               //遍历窗中的每一个数据
			if ((j + i*Wav_File_Work::FrameShift) > this->Get_dataNumber()) {
				dataFrameWindow[i][j] = 0;
			}
			dataFrameWindow[i][j] = dataDouble[j + i*Wav_File_Work::FrameShift] * Hamming_window(j);       //拷贝原数据列中的数据
		}
	}
	return true;
}

bool Wav_File_Work::kalmanFilter_filter(void)                                //用于将数据进行卡尔曼滤波
{
	double sumData = 0;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {
		sumData += dataDouble[i];
	}

	KalmanFilter *kalman = new KalmanFilter(sumData / Wav_File::Get_dataNumber(), 0);

	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {         //每个数据进入卡尔曼滤波器处理
		dataDouble[i] = kalman->kalmanFilter_filter(dataDouble[i]);
	}

	return true;
}

double* Wav_File_Work::DistributionSpace(unsigned long col)                  //分配一维数组空间
{
	double *tempspace = (double*)malloc(sizeof(double)*col);
	if (tempspace == NULL) {
		throw invalid_argument("ERROR : Memory failure !");
		return false;
	}
	return tempspace;
}

double** Wav_File_Work::DistributionSpace(unsigned long row, unsigned long col)        //分配二维数组空间
{
	double **tempspace = (double**)malloc(sizeof(double*)*row);
	if (tempspace == NULL) {
		throw invalid_argument("ERROR : Memory failure !");
		return false;
	}
	for (unsigned long i = 0; i < row; ++i) {
		tempspace[i] = (double*)malloc(sizeof(double)*col);
		if (tempspace[i] == NULL) {
			throw invalid_argument("ERROR : Memory failure !");
			return false;
		}
	}
	return tempspace;
}

void Wav_File_Work::ReleaseSpace(double **data, unsigned long row)           //释放二维数组空间
{
	for (unsigned long i = 0; i < row; ++i) {
		free(data[i]);
	}
	free(data);
}

void Wav_File_Work::Rader(double *data, unsigned long dataNumber)            //雷德算法，其中dataNumber一定是2^n
{
	unsigned long endNum;
	double temp;
	endNum = 0;
	for (unsigned long i = 0; i < dataNumber - 1; i++) {                     //因为第一个数永远是0不变，所以查找次数为N-1
		if (i < endNum) {                                                    //如果顺序数小于逆序数则交换，这是防止重复交换的一种手段，改为大于号也行
			temp = data[i];                                                  //交换过程
			data[i] = data[endNum];
			data[endNum] = temp;
		}
		unsigned long k = dataNumber >> 1;                                   //100 代表窗长的一半，用来判断最高位是否为1
		while (k <= endNum) {                                                //如果逆序数的最高位为1
			endNum = endNum - k;                                             //则当前位置0
			k >>= 1;                                                         //基标/2
		}
		endNum = endNum + k;                                                 //当前位为0，则逆转为1
	}
}

bool Wav_File_Work::DFT(double *data, unsigned long dataNumber)              //正向离散傅立叶变换，无虚部
{
	double exp;                                                              //这是指数部分
	double *dataDFT;
	dataDFT = DistributionSpace(dataNumber);
	for (unsigned long i = 0; i < dataNumber; ++i) {                         //一个一个建立新的数据
		dataDFT[i] = 0;                                                      //预防假数据
		exp = -2 * PI*i / dataNumber;                                        //指数部分
		for (unsigned long j = 0; j < dataNumber; ++j) {                     //做累加和
			dataDFT[i] += data[j] * cos(j*exp);                              //纯实数部分的做法
		}
	}
	for (unsigned long i = 0; i < dataNumber; ++i) {
		data[i] = dataDFT[i];                                                //拷贝数据
	}
	free(dataDFT);
	return true;
}

bool Wav_File_Work::IDFT(double *data, unsigned long dataNumber)             //逆向离散傅立叶变换，无虚部
{
	double exp;                                                              //这是指数部分
	double *dataIDFT;
	dataIDFT = DistributionSpace(dataNumber);
	for (unsigned long i = 0; i < dataNumber; ++i) {                         //一个一个建立新的数据
		dataIDFT[i] = 0;                                                     //预防假数据
		exp = -2 * PI*i / dataNumber;                                        //指数部分
		for (unsigned long j = 0; j < dataNumber; ++j) {                     //做累加和
			dataIDFT[i] += data[j] * cos(j*exp);                             //纯实数部分的做法
		}
		dataIDFT[i] /= dataNumber;                                           //逆向回归
	}
	for (unsigned long i = 0; i < dataNumber; ++i) {
		data[i] = dataIDFT[i];                                               //拷贝数据
	}
	free(dataIDFT);
	return true;
}

double* Wav_File_Work::FFT(double *data, unsigned long dataNumber)           //快速离散傅立叶变换，无虚部
{
	short power = 0;
	for (int i = dataNumber - 1; i > 0; ++power, i /= 2);

	fftNumber = (unsigned long)pow((float)2, (int)power);
	double *dataFFT = DistributionSpace(fftNumber);
	for (unsigned long i = 0; i < dataNumber; i++) {
		dataFFT[i] = data[i];                                                //初始化快速傅立叶变换数据
	}
	double *W = DistributionSpace(fftNumber / 2);                            //计算旋转因子
	for (unsigned long i = 0; i < fftNumber / 2; i++) {
		W[i] = cos(2 * i * PI / fftNumber);
	}
	unsigned long ulGroupLength = 1;										 //段的长度
	unsigned long ulHalfLength = 0;											 //段长度的一半
	unsigned long ulGroupCount = 0;											 //段的数量
	double cw, c1, c2;
	for (short b = 0; b < power; b++)                                        //计算FFT
	{
		ulHalfLength = ulGroupLength;
		ulGroupLength *= 2;
		for (unsigned long j = 0; j < fftNumber; j += ulGroupLength)
		{
			for (unsigned long k = 0; k < ulHalfLength; k++)
			{
				cw = W[k * fftNumber / ulGroupLength], dataFFT[j + k + ulHalfLength];
				c1 = dataFFT[j + k] + cw;
				c2 = dataFFT[j + k] - cw;
				dataFFT[j + k] = c1;
				dataFFT[j + k + ulHalfLength] = c2;
			}
		}
	}
	free(W);
	if (fftNumber == dataNumber) {                                           //如何FFT运算时个数正好，没有扩展，则需要拷贝数据
		for (unsigned long i = 0; i < dataNumber; ++i) {
			data[i] = dataFFT[i];
		}
		free(dataFFT);
		return NULL;
	}
	else {                                                                   //否则需要返回新的数据地址
		free(data);
		return dataFFT;
	}
}

bool Wav_File_Work::IFFT(double *data, unsigned long dataNumber)             //快速离散逆傅立叶变换，无虚部
{
	short power = 0;
	double *dataIFFT;
	for (unsigned long i = dataNumber - 1; i > 0; ++power, i /= 2);          //计算次幂
	for (unsigned long i = 1, j = dataNumber / 2; i < N - 1; ++i) {          //雷德算法重排位置
		if (i < j) {
			double temp = data[i];
			data[i] = data[j];
			data[j] = temp;
		}
		unsigned long k = dataNumber / 2;
		while (k <= j) {
			j = j - k;
			k = k / 2;
		}
		j = j + k;
	}
	dataIFFT = DistributionSpace(dataNumber);
	for (unsigned long i = 0; i < dataNumber; ++i) {                         //逆操作
		dataIFFT[i] = data[i] / dataNumber;
	}
	for (short p = 0; p < power; ++p) {                                      //IFFT计算
		for (unsigned long i = 0; i < pow((long double)2, (long double)p); ++i) {
			double cw = i*pow((long double)2, (long double)power - (p + 1)); //计算旋转因子
			for (unsigned long j = i; j < dataNumber - 1; j += (unsigned long)pow((long double)2, (long double)(p + 1))) {
				unsigned long index = j + (unsigned long)pow((long double)2, (long double)p);
				cw = dataIFFT[index] * cos(2 * Wav_File_Work::PI*cw / dataNumber);
				dataIFFT[index] = dataIFFT[j] - cw;
				dataIFFT[j] = dataIFFT[j] + cw;
			}
		}
	}
	for (unsigned long i = 0; i < dataNumber; ++i) {                         //拷贝数据到自身
		data[i] = dataIFFT[i];
	}
	free(dataIFFT);
	return true;
}

bool Wav_File_Work::FrameFFT(double** data, unsigned long frameNumber)       //用于计算分帧后的整体FFT
{
	cout << "TIP : Calculate FrameFFT ..." << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {                        //逐帧计算FFT
		double *space = this->FFT(data[i], Wav_File_Work::N);                //保存下返回的地址，因为这有可能是新数据的地址
		if (space != NULL) {
			data[i] = space;
		}
	}
	return true;
}
bool Wav_File_Work::FrameIFFT(double** data, unsigned long frameNumber)      //用于计算分帧后的整体IFFT
{
	cout << "TIP : Calculate FrameIFFT ..." << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {                        //逐帧计算IFFT
		this->IFFT(data[i], Wav_File_Work::N);
	}
	return true;
}

double Wav_File_Work::Mel_Filter(unsigned long data, int filterFlag)         //mel 三角滤波器         公式来自网络
{
	if (data < FilterFunction(filterFlag - 1) || data >= FilterFunction(filterFlag + 1)) {
		return 0;
	}
	else if (data >= FilterFunction(filterFlag - 1) && data <= FilterFunction(filterFlag)) {
		return 2 * (data - FilterFunction(filterFlag - 1)) /
			((FilterFunction(filterFlag + 1) - FilterFunction(filterFlag - 1))*
			(FilterFunction(filterFlag) - FilterFunction(filterFlag - 1)));
	}
	else if (data >= FilterFunction(filterFlag) && data <= FilterFunction(filterFlag + 1)) {
		return 2 * (FilterFunction(filterFlag - 1) - data) /
			((FilterFunction(filterFlag + 1) - FilterFunction(filterFlag - 1))*
			(FilterFunction(filterFlag) - FilterFunction(filterFlag - 1)));
	}
	else {
		return 0;
	}
}

double Wav_File_Work::FilterFunction(int flag)                               //用于计算三角滤波器频率的辅助函数      参数来自网络
{
	return 700 * (exp(log((long double)1 + 20000 / 700)*flag / (Wav_File_Work::FilterNumber + 1)) - 1);
}

bool Wav_File_Work::AutocorrelativeFunction(void)                            //计算每一帧的自相关函数，是求功率谱谱熵的其中一部分
{
	if (dataAutoFun) {
		ReleaseSpace(dataAutoFun, frameNumber);
	}
	dataAutoFun = DistributionSpace(frameNumber, Wav_File_Work::N);
	for (unsigned long i = 0; i < frameNumber; ++i) {
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			dataAutoFun[i][j] = 0;
			for (unsigned long k = 0; k < Wav_File_Work::N - j; ++k) {
				dataAutoFun[i][j] += dataFrameWindow[i][k] * dataFrameWindow[i][k + j];
			}
		}
	}
	return true;
}

bool Wav_File_Work::PSDF(void)                                               //计算归一化的功率谱密度函数
{
	if (dataPSDF) {
		ReleaseSpace(dataPSDF, frameNumber);
	}
	dataPSDF = DistributionSpace(frameNumber, Wav_File_Work::N);
	for (unsigned long i = 0; i < frameNumber; ++i) {
		double sum = 0;
		for (unsigned long j = 0; j < Wav_File_Work::N / 2; ++j) {
			sum += dataAutoFun[i][j];
		}
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			dataPSDF[i][j] = dataAutoFun[i][j] / sum;
		}
	}
	return true;
}

bool Wav_File_Work::NormallizationFun(double *data, unsigned long dataNumber)
{
	double max = 0;
	for (unsigned long i = 0; i < dataNumber; ++i) {
		double tempdata = data[i] > 0 ? data[i] : -data[i];
		if (tempdata<pow((double)10, -3) || tempdata>pow((double)10, 3)) {
			data[i] = 0;
		}
		if (tempdata > max) {
			max = tempdata;
		}
	}
	for (unsigned long i = 0; i < dataNumber; ++i) {
		data[i] /= max;
	}
	return true;
}

void Wav_File_Work::Normalization(void)                                      //将重点数据归一化操作
{
	if (dataDouble) {
		this->NormallizationFun(dataDouble, Wav_File::Get_dataNumber());
	}
	if (dataCepsDistance) {
		this->NormallizationFun(dataCepsDistance, frameNumber);
	}
	if (dataEngChart) {
		this->NormallizationFun(dataEngChart, frameNumber);
	}
	if (dataEngDevia) {
		this->NormallizationFun(dataEngDevia, frameNumber);
	}
	if (dataSpectrumEnt) {
		this->NormallizationFun(dataSpectrumEnt, frameNumber);
		for (unsigned long i = 0; i < frameNumber; ++i) {
			dataSpectrumEnt[i] = dataSpectrumEnt[i] * 100 - (unsigned long)(dataSpectrumEnt[i] * 100);
			if (_finite(dataSpectrumEnt[i]) == 0) {
				dataSpectrumEnt[i] = 0;
			}
		}
	}
}

void Wav_File_Work::FrameVoice(double *voice)                                //按照一定的计算方式计算每一帧的标准值, 先使用平均值
{
	for (unsigned long i = 0; i < frameNumber; ++i) {
		voice[i] = 0;
		for (unsigned long j = 0; j < this->N; ++j) {
			voice[i] += this->dataFrameWindow[i][j];
		}
		voice[i] /= this->N;
	}
}

int Wav_File_Work::Mute_State_Process(FeatureParameter framePoint)           //无语音状态处理
{
	bool logic_A = fabs(framePoint.engChart) > this->threshold_l_EngChart;
	bool logic_B = framePoint.cepsDistance > this->threshold_l_CepsDistance;
	bool logic_C = framePoint.engDevia > this->threshold_l_EngDevia;
	bool logic_D = framePoint.spectrumEnt > this->threshold_l_SpectrumEnt;

	if ((logic_A && logic_B) || (logic_A && logic_C) || (logic_B && logic_C) ||
		(logic_A && logic_D) || (logic_C && logic_D) || (logic_B && logic_D)) {        //这是一个逻辑表达式，表示其中满足条件达到两个或以上则条件成功
		return _PARAGRAPH_INTERIM;
	}
	else {
		return _PARAGRAPH_MUTE;
	}
}

int Wav_File_Work::Interim_State_Process(FeatureParameter framePoint, unsigned long length)      //语音假设状态处理
{
	//1） to _PARAGRAPH_MUTE
	if (this->Mute_State_Process(framePoint) == _PARAGRAPH_MUTE) {
		return _PARAGRAPH_MUTE;
	}
	else {
		//2） to _PARAGRAPH_INTERIM
		if (length < this->interim_StateLength) {
			return _PARAGRAPH_INTERIM;
		}
		else {
			//3） to _PARAGRAPH_VOICE
			return _PARAGRAPH_VOICE;
		}
	}
}

int Wav_File_Work::Voice_State_Process(FeatureParameter framePoint)          //语音状态处理
{
	bool logic_A = (fabs(framePoint.engChart) > this->threshold_l_EngChart) &&
		(framePoint.engChart < this->threshold_h_EngChart);
	bool logic_B = (framePoint.cepsDistance > this->threshold_l_CepsDistance) &&
		(framePoint.cepsDistance < this->threshold_h_CepsDistance);
	bool logic_C = (framePoint.engDevia > this->threshold_l_EngDevia) &&
		(framePoint.engDevia < this->threshold_h_EngDevia);
	bool logic_D = (framePoint.spectrumEnt > this->threshold_l_SpectrumEnt) &&
		(framePoint.spectrumEnt < this->threshold_h_SpectrumEnt);

	if ((logic_A && logic_B) || (logic_A && logic_C) || (logic_B && logic_C) ||
		(logic_A && logic_D) || (logic_C && logic_D) || (logic_B && logic_D)) {        //这是一个逻辑表达式，表示其中满足条件达到两个或以上则条件成功
			//满足条件，则继续保持语音状态
		return _PARAGRAPH_VOICE;
	}
	else {
		//不在条件之中
		return _PARAGRAPH_BURST;
	}
}

int Wav_File_Work::Burst_State_Process(FeatureParameter framePoint, unsigned long length)        //爆破音/无语音状态处理
{
	//1) to _PARAGRAPH_MUTE
	if (length > this->burst_StateLength) {
		return _PARAGRAPH_MUTE;
	}
	else {
		//2) to _PARAGRAPH_VOICE
		if (this->Voice_State_Process(framePoint) == _PARAGRAPH_VOICE) {
			return _PARAGRAPH_VOICE;
		}
		else {
			//3) to _PARAGRAPH_BURST
			return _PARAGRAPH_BURST;
		}
	}
}

void Wav_File_Work::ShowEndpoint()                                           //使用这个函数获取所有的段落数据显示
{
	for (unsigned long i = 0; i < this->voiceNumber; ++i) {
		VoiceParagraph tempParagraph = this->Get_dataVoicePoint(i);
		cout << tempParagraph.begin << " " << tempParagraph.end << " " << tempParagraph.voiceLength << endl;
	}
}

double Wav_File_Work::Get_dataNumber(void)                                   //获取Double数据的个数
{
	return Wav_File::Get_dataNumber();
}

double Wav_File_Work::Get_DataDouble(unsigned long Number)                   //获取转换后的Double数据
{
	if (Number >= (Wav_File::Get_dataNumber()) || Number < 0) {              //如果所需要的数超过了数据个数
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataDouble[Number];
	}
}

int Wav_File_Work::Get_WindowLength(void)                                    //获取帧长（窗的大小）
{
	return N;
}

unsigned long Wav_File_Work::Get_FrameNumber(void)                           //获取帧数
{
	return frameNumber;
}

double Wav_File_Work::Get_DataCepsDistance(unsigned long Number)             //获取指定位置上的倒谱距离
{
	if (Number >= frameNumber || Number < 0) {
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataCepsDistance[Number];
	}
}
double Wav_File_Work::Get_DataEngChart(unsigned long Number)                 //获取指定位置上的能量谱
{
	if (Number >= frameNumber || Number < 0) {
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataEngChart[Number];
	}
}
double Wav_File_Work::Get_DataEngDevia(unsigned long Number)                 //获取指定位置上的能量谱方差
{
	if (Number >= frameNumber || Number < 0) {
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataEngDevia[Number];
	}
}
double Wav_File_Work::Get_DataSpectrumEnt(unsigned long Number)              //获取指定位置上的功率谱谱熵
{
	if (Number >= frameNumber || Number < 0) {
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
		return -1;
	}
	else {
		return dataSpectrumEnt[Number];
	}
}


unsigned long Wav_File_Work::Get_voiceNumber(void)                           //获取语音段落个数
{
	return voiceNumber;
}

VoiceParagraph Wav_File_Work::Get_dataVoicePoint(unsigned long Number)       //获取某个语音点
{
	if (Number >= voiceNumber || Number < 0) {                               //如果所需要的数超过了数据个数
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		throw invalid_argument("ERROR : Over list !");
	}
	else {
		return voiceParagraph[Number];
	}
}

void Wav_File_Work::ShowData(void)                                           //覆盖父类的展示数据函数
{
	/*
	int max = 0, min = 0;
	cout << "TIP : Double data " << endl;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {
		cout << dataDouble[i] << "\t";
		if (dataDouble[i] > dataDouble[max]) {
			max = i;
		}
		if (dataDouble[i] < dataDouble[min]) {
			min = i;
		}
	}
	cout << endl;
	*/
	/*
	cout << "Max " << dataDouble[max] << endl;
	cout << "Min " << dataDouble[min] << endl;
	cout << "TIP : Parameter " << endl;
	cout << "Max energy " << Get_maxEnergy() << endl;
	cout << "Min energy " << Get_minEnergy() << endl;
	cout << "Max ZCR " << Get_maxZCR() << endl;
	cout << "Min ZCR " << Get_minZCR() << endl;

	cout << "TIP : Energy " << endl;
	for (auto i : dataEnergy) {
	cout << i << "\t";
	}
	cout << endl;

	cout << "TIP : ZCR " << endl;
	for (auto i : dataZCR) {
	cout << i << "\t";
	}
	cout << endl;

	for (int i = 0; i < (Wav_File::Get_dataNumber() - N); ++i) {
	cout << Get_DataEnergy(i) << endl;
	}

	for (int i = 0; i < (Wav_File::Get_dataNumber() - N); ++i) {
	cout << Get_DataZCR(i) << endl;
	}
	*/

	/*
	cout << "TIP : DFT " << endl;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {
	cout << dataDFT[i] << "\t";
	}
	cout << endl;
	*/
	/*
	cout << "TIP : FFT " << endl;
	for (unsigned long i = 0; i < fftNumber; ++i) {
	cout << dataFFT[i] << "\t";
	}
	cout << endl;
	*/
	/*
	cout << "TIP : Cepstrum " << endl;
	for (unsigned long i = 0; i < Wav_File::Get_dataNumber(); ++i) {
	cout << dataCeps[i] << "\t";
	}
	cout << endl;
	*/

	/*
	for (unsigned long i = 0; i < frameNumber; ++i) {
	for (int j = 0; j < Wav_File_Work::CoefficientOrder; ++j) {
	cout << dataMfcc[i][j] << "\t";
	}
	cout << endl;
	}
	*/

	/*
	cout << "倒谱距离数据：" << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {
	cout<< dataCepsDistance[i] << "\t";
	}
	cout << endl;
	cout << "能量谱数据：" << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {
	cout << dataEngChart[i] << "\t";
	}
	cout << endl;
	*/
	/*
	cout << "能量方差数据：" << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {
		dataEngDevia[i] *= 10;
		dataEngDevia[i] = dataEngDevia[i] - (int)dataEngDevia[i];
		cout << dataEngDevia[i] << "\t";
	}
	cout << endl;
	*/

	cout << "谱熵数据：" << endl;
	for (unsigned long i = 0; i < frameNumber; ++i) {
		cout << dataSpectrumEnt[i] << "\t";
	}
	cout << endl;

}

void Wav_File_Work::SaveNewWav(char* path)                                    //保存去掉空白处的语音文件
{
	Wav_File::SaveNewWav(voiceNumber, voiceParagraph, Wav_File_Work::FrameShift, path);                //调用父类的生成函数
}

void Wav_File_Work::SaveParagraphInfo(char* path)                            //保存检测信息到文件
{
	ofstream paragraphData(path, ios::out);
	if (paragraphData.is_open()) {
		unsigned long sumLength = 0;
		paragraphData << "No | Begin | End | Length" << endl;
		for (unsigned long i = 0; i < this->Get_voiceNumber(); ++i) {
			VoiceParagraph tempPoint = this->Get_dataVoicePoint(i);
			sumLength += tempPoint.voiceLength;
			paragraphData << (i + 1) << " | " << tempPoint.begin << " | " << tempPoint.end << " | " << tempPoint.voiceLength << endl;
		}
		paragraphData << endl;
		paragraphData << "Paragraph number : " << this->Get_voiceNumber() << endl;
		paragraphData << "Paragraph data number : " << this->Get_FrameNumber() << endl;
		paragraphData << "Paragraph useful length : " << sumLength << endl << endl;

		paragraphData << "Settings" << endl;
		paragraphData << "----------------------------" << endl;
		paragraphData << "predict_q : " << KalmanFilter::predict_q << endl;
		paragraphData << "newMeasured_q : " << KalmanFilter::newMeasured_q << endl;
		paragraphData << "N : " << Wav_File_Work::N << endl;
		paragraphData << "FrameShift : " << Wav_File_Work::FrameShift << endl;
		paragraphData << "PI : " << Wav_File_Work::PI << endl;
		paragraphData << "FilterNumber : " << Wav_File_Work::FilterNumber << endl;
		paragraphData << "CoefficientOrder : " << Wav_File_Work::CoefficientOrder << endl;
		paragraphData << "threshold_h_CepsDistance : " << Wav_File_Work::threshold_h_CepsDistance << endl;
		paragraphData << "threshold_h_EngChart : " << Wav_File_Work::threshold_h_EngChart << endl;
		paragraphData << "threshold_h_EngDevia : " << Wav_File_Work::threshold_h_EngDevia << endl;
		paragraphData << "threshold_h_SpectrumEnt : " << Wav_File_Work::threshold_h_SpectrumEnt << endl;
		paragraphData << "threshold_l_CepsDistance : " << Wav_File_Work::threshold_l_CepsDistance << endl;
		paragraphData << "threshold_l_EngChart : " << Wav_File_Work::threshold_l_EngChart << endl;
		paragraphData << "threshold_l_EngDevia : " << Wav_File_Work::threshold_l_EngDevia << endl;
		paragraphData << "threshold_l_SpectrumEnt : " << Wav_File_Work::threshold_l_SpectrumEnt << endl;
		paragraphData << "interim_StateLength : " << Wav_File_Work::interim_StateLength << endl;
		paragraphData << "burst_StateLength : " << Wav_File_Work::burst_StateLength << endl;
	}
	else {
		cout << "ERROR : Paragraph write open fail !" << endl;
	}
}

bool Wav_File_Work::Cepstrum(void)                                           //倒频谱
{
	cout << "TIP : Calculate CepsTrum ..." << endl;
	if (frameNumber < 1) {                                                   //如果检测无帧的处理
		throw invalid_argument("ERROR : No frame !");
		return false;
	}

	double **tempData = DistributionSpace(frameNumber, Wav_File_Work::N);

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //拷贝数据到处理数组，保护原有数据
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = dataFrameWindow[i][j];
		}
	}

	this->FrameFFT(tempData, frameNumber);                                   //对帧数据进行FFT
	for (unsigned long i = 0; i < frameNumber; ++i) {
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = log(fabs(tempData[i][j]));                      //求模对数
		}
	}

	dataCeps = DistributionSpace(frameNumber, Wav_File_Work::N);

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //拷贝数据到结果数组
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			dataCeps[i][j] = tempData[i][j];
		}
	}

	ReleaseSpace(tempData, frameNumber);

	this->FrameIFFT(dataCeps, frameNumber);                                  //对帧数据进行IFFT
	return true;
}

bool Wav_File_Work::Pitch(void)                                              //基音周期
{
	cout << "TIP : Calculate Pitch ..." << endl;
	int Pmax = Wav_File::Get_SampleRate() / 60;                              //检测的最大限
	int Pmin = Wav_File::Get_SampleRate() / 500;                             //检测的最低限
	if (dataPitch) {                                                         //如果原来有数据则此为重做计划
		free(dataPitch);                                                     //擦除原来的数据
	}
	dataPitch = (int*)malloc(sizeof(int)*frameNumber);                       //为数据分配空间
	if (dataPitch == NULL) {
		throw invalid_argument("ERROR : Memory failure !");
		return false;
	}

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //每一帧都要找到一个最大值
		unsigned long max = 0;
		for (int j = Pmin; j < Pmax; ++j) {                                  //限制查找范围
			if (dataCeps[i][j] >= dataCeps[i][max]) {                        //找到数据的最大值
				max = j;
			}
		}
		dataPitch[i] = max;
	}
	return true;
}

bool Wav_File_Work::MFCC(void)                                               //梅尔倒谱系数
{
	cout << "TIP : Calculate MFCC ..." << endl;
	if (frameNumber < 1) {                                                   //无帧时的紧急处理
		throw invalid_argument("ERROR : No frame !");
		return false;
	}

	double **tempData = DistributionSpace(frameNumber, Wav_File_Work::N);

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //拷贝数据到处理数组，保护原有数据
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = dataFrameWindow[i][j];
		}
	}

	this->FrameFFT(tempData, frameNumber);                                   //对帧数据进行FFT
	for (unsigned long i = 0; i < frameNumber; ++i) {
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = pow(tempData[i][j], 2);                         //平方操作
		}
	}

	double **temp_dataMfcc = DistributionSpace(frameNumber, Wav_File_Work::FilterNumber);        //为数据分配空间

	for (unsigned long i = 0; i < frameNumber; ++i) {
		for (int m = 0; m < Wav_File_Work::FilterNumber; ++m) {              //每个数据通过FilterNumber个三角滤波器
			temp_dataMfcc[i][m] = 0;
			for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {           //累加和计算数据
				temp_dataMfcc[i][m] += tempData[i][j] * this->Mel_Filter(j, m);
			}
		}
	}
	ReleaseSpace(tempData, frameNumber);

	dataMfcc = DistributionSpace(frameNumber, Wav_File_Work::CoefficientOrder);        //为数据分配空间

	for (unsigned long i = 0; i < frameNumber; ++i) {
		for (int n = 0; n < Wav_File_Work::CoefficientOrder; ++n) {          //分为CoefficientOrder个阶数
			dataMfcc[i][n] = 0;
			for (int j = 0; j < Wav_File_Work::FilterNumber; ++j) {          //公式求取mfcc
				double tempdataA = fabs(temp_dataMfcc[i][j]);
				double tempdataB = cos((n + 1)*Wav_File_Work::PI*(j - 0.5) / Wav_File_Work::FilterNumber);
				if (_finite(log(tempdataA)* tempdataB) == 0) {
					continue;
				}
				dataMfcc[i][n] += log(tempdataA)* tempdataB;
			}
			dataMfcc[i][n] *= sqrt((long double)2 / Wav_File_Work::FilterNumber);
		}
	}

	ReleaseSpace(temp_dataMfcc, frameNumber);
	return true;
}

bool Wav_File_Work::CepstrumDistance(void)                                   //计算倒谱距离（严重注意-运行前必须先计算mel倒谱系数MFCC）
{
	cout << "TIP : Calculate Cepstrum Distance ..." << endl;
	dataCepsDistance = DistributionSpace(frameNumber);                       //为倒谱距离数据分配空间
	double *tempCeps = DistributionSpace(Wav_File_Work::CoefficientOrder);   //为噪音数据分配空间
	for (int i = 0; i < Wav_File_Work::CoefficientOrder; ++i) {              //取前5帧计算背景噪音系数
		tempCeps[i] = 0;
		for (int j = 0; j < 5; ++j) {
			tempCeps[i] += dataMfcc[j][i];
		}
		tempCeps[i] /= 5;
	}

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //逐帧计算倒谱距离
		dataCepsDistance[i] = 0;
		for (int j = 0; j < Wav_File_Work::CoefficientOrder; ++j) {
			dataCepsDistance[i] += pow(dataMfcc[i][j] - tempCeps[j], 2);
		}
		dataCepsDistance[i] = 4.3429*sqrt(pow(dataMfcc[i][0] - tempCeps[0], 2) + 2 * dataCepsDistance[i]);
	}
	return true;
}

bool Wav_File_Work::EnergySpectrum(void)                                     //计算短时能量
{
	cout << "TIP : Calculate Energy Spectrum ..." << endl;
	dataEngChart = DistributionSpace(frameNumber);
	double **tempData = DistributionSpace(frameNumber, Wav_File_Work::N);

	for (unsigned long i = 0; i < frameNumber; ++i) {                        //拷贝数据到处理数组，保护原有数据
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = dataFrameWindow[i][j];
		}
	}

	this->FrameFFT(tempData, frameNumber);                                   //对帧数据进行FFT
	for (unsigned long i = 0; i < frameNumber; ++i) {
		dataEngChart[i] = 0;
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			dataEngChart[i] += tempData[i][j];
		}
	}

	ReleaseSpace(tempData, frameNumber);
	return true;
}

bool Wav_File_Work::EnergyDeviation(void)                                    //计算能量谱方差
{
	cout << "TIP : Calculate Energy Deviation ..." << endl;
	dataEngDevia = DistributionSpace(frameNumber);
	double **tempData = DistributionSpace(frameNumber, Wav_File_Work::N);
	for (unsigned long i = 0; i < frameNumber; ++i) {                        //拷贝数据到处理数组，保护原有数据
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			tempData[i][j] = dataFrameWindow[i][j];
		}
	}

	this->FrameFFT(tempData, frameNumber);
	for (unsigned long i = 0; i < frameNumber; ++i) {
		dataEngDevia[i] = 0;
		double tempEngChart = 0;
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {                //重新计算EngChart，因为如果不加任何处理，EngChart数值将会很小
			tempEngChart += fabs(tempData[i][j]);
		}
		for (unsigned long j = 0; j < Wav_File_Work::N; ++j) {
			dataEngDevia[i] += pow(fabs(tempData[i][j]) - tempEngChart / Wav_File_Work::N, 2);
		}
	}

	ReleaseSpace(tempData, frameNumber);
	return true;
}

bool Wav_File_Work::SpectrumEntropy(void)                                    //计算谱熵
{
	cout << "TIP : Calculate Spectrum Entropy ..." << endl;
	this->AutocorrelativeFunction();                                         //求每一帧的短时自相关函数
	this->FrameFFT(dataAutoFun, frameNumber);                                //对短时自相关函数进行快速傅里叶变换，得到语音帧的短时功率谱密度
	this->PSDF();                                                            //计算这一帧中的每个频率分量的归一化功率谱密度函数
	if (dataSpectrumEnt) {
		free(dataSpectrumEnt);
	}
	dataSpectrumEnt = DistributionSpace(frameNumber);
	for (unsigned long i = 0; i < frameNumber; ++i) {
		dataSpectrumEnt[i] = 0;
		for (unsigned long j = 0; j < Wav_File_Work::N / 2; ++j) {
			dataSpectrumEnt[i] += dataPSDF[i][j] * log(dataPSDF[i][j]);      //依据归一化的功率谱密度，计算语音信息熵
		}
		dataSpectrumEnt[i] *= -1;
	}
	return true;
}

bool Wav_File_Work::Endpoint_Detection(void)                                 //is a voise end point check function
{
	int statusFlag = 0;                                                      //设置语音状态标志
	unsigned long begin = 0;                                                 //语音段落的起点
	unsigned long end = 0;                                                   //语音段落的终点
	unsigned long voiceLength = 0;                                           //语音段落的长度

	unsigned long suppose_V_Length = 0;                                      //假设状态中语音持续的长度
	unsigned long burse_V_Length = 0;                                        //爆破状态中语音持续的长度
	bool voiceJudge = true;                                                  //如果为true则语音段落正常结束，如果为false，则说明语音段落还没有结束，用于终点判断

	voiceNumber = 0;

	statusFlag = _PARAGRAPH_MUTE;                                            //初始时语音处于静音段

	//计算voice的标准量
	double *voice = this->DistributionSpace(frameNumber);                    //为voice分配空间
	this->FrameVoice(voice);

	for (unsigned long i = 0; i < frameNumber; ++i) {
		FeatureParameter feature(voice[i], this->dataEngChart[i], this->dataCepsDistance[i],
			this->dataEngDevia[i], this->dataSpectrumEnt[i]);

		switch (statusFlag)
		{
		case _PARAGRAPH_MUTE:
			statusFlag = this->Mute_State_Process(feature);
			//1) => _PARAGRAPH_MUTE

			//2) => _PARAGRAPH_INTERIM
			if (statusFlag == _PARAGRAPH_INTERIM) {                          //即将转换到准备阶段
				suppose_V_Length = 0;                                        //初始化准备阶段的持续时间长度
			}
			break;
		case _PARAGRAPH_INTERIM:
			statusFlag = this->Interim_State_Process(feature, suppose_V_Length);
			//1) => _PARAGRAPH_MUTE

			//2) => _PARAGRAPH_INTERIM
			++suppose_V_Length;                                              //若持续此状态，则添加准备时间的长度

			//3) => _PARAGRAPH_VOICE
			if (statusFlag == _PARAGRAPH_VOICE) {                            //即将转换到语音阶段
				begin = i - suppose_V_Length;                                //则从准备阶段开始时的帧数则为起点
			}
			break;
		case _PARAGRAPH_VOICE:
			voiceJudge = false;                                              //语音开始了
			statusFlag = this->Voice_State_Process(feature);
			//1) => _PARAGRAPH_VOICE
			++voiceLength;
			//2) => _PARAGRAPH_BURST
			if (statusFlag == _PARAGRAPH_BURST) {                            //即将跳转到爆破音处，为计算爆破音的长度做准备
				burse_V_Length = 0;
			}
			break;
		case _PARAGRAPH_BURST:
			statusFlag = this->Burst_State_Process(feature, burse_V_Length);
			//1） => _PARAGRAPH_BURST
			++burse_V_Length;
			//2） => _PARAGRAPH_VOICE

			//3） => _PARAGRAPH_MUTE
			//this is a end point
			if (statusFlag == _PARAGRAPH_MUTE) {
				end = i - burse_V_Length;
				voiceLength = end - begin;
				VoiceParagraph voiceTemp(begin, end, voiceLength);
				this->voiceParagraph.push_back(voiceTemp);                   //保存这一段点
				this->voiceNumber++;
				voiceJudge = true;
			}
			break;
		default:break;                                                       //不可能的情况,出错
		}
	}
	if (!voiceJudge) {
		VoiceParagraph voiceTemp(begin, frameNumber, frameNumber - begin);
		this->voiceParagraph.push_back(voiceTemp);
		this->voiceNumber++;
	}
	return true;
}

