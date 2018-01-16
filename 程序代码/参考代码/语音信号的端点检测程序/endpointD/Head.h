//语音文件的格式：
//文件名：	*.dat
//文件格式：	二进制流文件
//文件结构：采用索引格式区分每句话
//    字节数：0～sizeof(dataHEAD)	内容：structure dataHEAD
//	______________________文件头______________________
/*
typedef struct 
{
	long		Version;		//版本号，本次为 20001120
	char		Name[15];       	// 姓名
	char		Sex[4];         	// 性别
	char		Age[15];		// 年龄
	char 		HomeTown[30];  		//方言类型，例如"上海普通话"
	char		Date[24]; 		// 日期
	char		Group[10];		//00 组别 
	short		nSentenceNum;		//句子数
	long		SampleRate;		// 采样率
	char		ParameterType[10];	//参数类型（WAVE）
	short		SampleType;		//数据类型(1=char;2=short;3=long;4=float;5=double) 
	char		Reservation[76];	// 保留位
} dataHEAD;
*/
//    字节数：198～199		内容：文件中语音句子数，是short 型


//****
//---------------------- 索引结构 --------------------------------------

//INDEX 从 200 字节开始。索引结构为：
/*
typedef struct {
	long	Index;
	long	Length;
} dataADDRESS;
*/
//其中Length表示该句话所包含的字节数




//************************
//特征文件的格式为

//文件名：	*mfcc.dat  ( *lpcc.dat)
//文件格式：	二进制流文件
//文件结构：采用索引格式区分每句话
//    字节数：0～sizeof(dataHEAD)	内容：structure dataHEAD
//	______________________文件头______________________


typedef struct 
{
	long		Version;		//版本号，本次为 20001120
	char		Name[15];       	// 姓名
	char		Sex[4];         	// 性别
	char		Age[15];		// 年龄
	char 		HomeTown[30];  		//方言类型，例如"上海普通话"
	char		Date[24]; 		// 日期
	char		Group[10];		//00 组别 
	short		nSentenceNum;		//句子数
	long		SampleRate;		// 采样率
	char		ParameterType[10];	//参数类型（MFCC/LPCC）
	short		SampleType;		//数据类型(1=char;2=short;3=long;4=float;5=double) 
	short		Feature_Order;		//特征维数
	char		Reservation[74];	// 保留位
} dataHEAD;

//    字节数：198～199		内容：文件中语音句子数，是short 型


//---------------------- 索引结构 --------------------------------------

//INDEX 从 200 字节开始。索引结构为：

typedef struct {
	long	Index;
	long	FrameAmount;
} dataADDRESS;
//其中FrameAmount表示该句话所包含的帧数




