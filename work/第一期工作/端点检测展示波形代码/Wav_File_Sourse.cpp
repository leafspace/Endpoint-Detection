#include "Wav_File_Header.h"
#include "stdafx.h"

bool Wav_File::Conversion_Tuple(void)
{
	try
	{
		cout << "TIP : Change to real format data ..." << endl;
		dataNumber = dataSize / chunkAlign;                                  //求出数据个数
		dataTuple = (long*)malloc(sizeof(long)*dataNumber);                  //为新字节数据空间赋值
		long tempNum = 0;                                                    //用来保存每个新字节数据
		for (unsigned long i = 0; i < dataNumber; ++i) {                     //总共有dataNumber个数据
			int Flag = 0;
			for (short j = 0; j < chunkAlign; ++j) {                         //每个数据有两个字节
				tempNum = MakeWord((long)data[i*chunkAlign + j], tempNum);   //每次将tempNum做为高字节数据，新字节为低字节数据制作新数据				
			}
			dataTuple[i] = tempNum;                                          //保存下新字节数据
			tempNum = 0;                                                     //初始化缓冲区操作

			if (dataMax < dataTuple[i]) {                                    //求出最大值
				dataMax = dataTuple[i];
			}
			if (dataMin > dataTuple[i]) {                                    //求出最小值
				dataMin = dataTuple[i];
			}
		}
	}
	catch (invalid_argument &e) {
		cerr << e.what() << endl;
		MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
		return false;
	}
	return true;
}

long Wav_File::MakeWord(long NumberA, long NumberB)                          //合并字节，NumberA表示高字节位，NumberB表示低字节位
{
	int Flag = 0;                                                            //标志，用来表示最后的数的正负，1为负，0为正
	if (NumberA >= 0) {
		if (NumberB >= 0) {
			return NumberA * 256 + NumberB;                                  //高低字节都是正数，则直接移位合并
		}
		else {
			return NumberA * 256 + abs(NumberB) + 128;                       //高字节为正数，低字节为负数
		}
	}
	else {
		Flag = 1;
		if (NumberB > 0) {                                                   //高字节为负数，低字节为正数
			return -1 * (abs(NumberA) * 256 + NumberB);
		}
		else {                                                               //高低字节都是负数
			return -1 * (abs(NumberA) * 256 + abs(NumberB) + 128);
		}
	}
}

bool Wav_File::Read_File(void)
{
	try
	{
		cout << "TIP : Reading file ..." << endl;
		fread(id_RIFF, sizeof(char), 4, fp);                                   //读取'RIFF'
		id_RIFF[4] = 0;                                                        //末尾添零
		if (strcmp(id_RIFF, "RIFF")) {
			throw invalid_argument("ERROR : File not RIFF file !");
		}
		fread(&fileSize, sizeof(unsigned long), 1, fp);                        //读取文件的大小
		fread(id_WAVE, sizeof(char), 4, fp);                                   //读取'WAVE'
		id_WAVE[4] = 0;
		if (strcmp(id_WAVE, "WAVE")) {
			throw invalid_argument("ERROR : File not WAVE file !");
		}
		fread(id_FMT, sizeof(char), 4, fp);                                    //读取'FMT'
		id_FMT[3] = 0;
		fread(&formatLength, sizeof(unsigned long), 1, fp);                    //
		fread(&formatTag, sizeof(short), 1, fp);                               //读取文件标签
		fread(&channelsNumber, sizeof(short), 1, fp);                          //读取通道数目
		fread(&sampleRate, sizeof(unsigned long), 1, fp);                      //读取采样频率
		fread(&secondBytes, sizeof(unsigned long), 1, fp);                     //读取每秒数据量
		fread(&chunkAlign, sizeof(short), 1, fp);                              //读取块对其
		fread(&sampleBits, sizeof(short), 1, fp);                              //读取样本大小
		if (formatLength > 16) {
			fread(&appendInformation, sizeof(short), 1, fp);                   //读取附加信息
		}
		fread(id_DATA, sizeof(char), 4, fp);                                   //读取'DATA'
		id_DATA[4] = 0;
		fread(&dataSize, sizeof(unsigned long), 1, fp);                        //读取数据大小
		data = (char*)malloc(sizeof(char)*dataSize);                           //申请数据的存储空间
		fread(data, sizeof(char), dataSize, fp);                               //读取数据
	}
	catch (invalid_argument &e)
	{
		MessageBoxA(NULL, e.what(), "ERROR", MB_ICONHAND);
		return false;
	}
	if (Conversion_Tuple() == true) {                                          //转换格式成功
		MessageBoxA(NULL, "TIP : File read ok !", "TIP", MB_ICONASTERISK);
	}
	else {
		MessageBoxA(NULL, "ERROR : Data change failure !", "ERROR", 0);
		return false;
	}

	return true;
}

unsigned long Wav_File::Get_SampleRate(void)                                 //获取采样频率
{
	return sampleRate;
}

short Wav_File::Get_ChunkAlign(void)                                         //获取样本字节数
{
	return chunkAlign;
}

long Wav_File::Get_Data(unsigned long Number)                                //获取某个位置上的数据
{
	if (Number >= dataNumber) {                                              //如果所需要的数超过了数据个数
		MessageBoxA(NULL, "ERROR : Over list !", "ERROR", MB_ICONHAND);
		return -1;
	}
	else {
		return dataTuple[Number];
	}
}

unsigned long Wav_File::Get_dataNumber(void)                                 //获取样本数据个数
{
	return dataNumber;
}

void Wav_File::ShowData(void) 
{
	cout << "TIP : Show data ..." << endl;
	cout << id_RIFF << endl;
	cout << fileSize << endl;
	cout << id_WAVE << endl << endl;

	cout << id_FMT << endl;
	cout << formatLength << endl;
	cout << formatTag << endl;
	cout << channelsNumber << endl;
	cout << sampleRate << endl;
	cout << secondBytes << endl;

	cout << chunkAlign << endl;
	cout << sampleBits << endl;
	cout << appendInformation << endl << endl;

	cout << id_FACT << endl;
	cout << appendLength << endl;
	cout << appendNone << endl << endl;

	cout << id_DATA << endl;
	cout << dataSize << endl;

	for (unsigned int i = 0; i < dataNumber; ++i) {
		cout << dataTuple[i] << "\t";
	}
	cout << endl;

	cout << "MAX :" << dataMax << endl;
	cout << "MIN :" << dataMin << endl;
}