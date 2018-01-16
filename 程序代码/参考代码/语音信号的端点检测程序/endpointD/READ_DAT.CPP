/**************************************************************************************************
	功能：			包含对于 .dat 文件的读操作
					读出句子数
					读出某句的长度
					读出某句的特征
	作者：			陈一宁
	修改记录：		
***************************************************************************************************/


#include	"stdlib.h"
#include	"stdio.h"
#include	"string.h"
/*========================================================================================================================

		函数名称：	Read_Length
		函数功能：	从，dat文件中读出一句话的特征帧数或者是字节数
		函数性质：	公共函数
		输入参数：	char *File_Name	文件名
					short Sentence_Num	第几句话
		输出参数：	无
		返回值：	该句话的帧数
		备注：		

		基本思路：		
========================================================================================================================*/
long Read_Length(char *File_Name,short Sentence_Num)
{
	FILE *Fp;
	Fp	= fopen(File_Name,"rb");

	if(Fp==NULL)
		return 0;
	
	long Off_Set;//每句话的起始地址
	long FrameAmount;//每句话的帧数或字节数

	fseek(Fp,200,SEEK_SET);//索引区从200开始
	fseek(Fp,2*sizeof(long)*Sentence_Num,SEEK_CUR);//到达该句子对应的索引处
	fread(&Off_Set,sizeof(long),1,Fp);
	fread(&FrameAmount,sizeof(long),1,Fp);

	fclose(Fp);
	return FrameAmount;
}
/*========================================================================================================================

		函数名称：	Read_Dat
		函数功能：	从，dat文件中读出一句话的特征
		函数性质：	公共函数
		输入参数：	char *File_Name	文件名
					short Sentence_Num	第几句话
					short Feature_Order	特征的维数
		输出参数：	float **Feature	该句的特征
		返回值：	该句话的帧数
		备注：		

		基本思路：		
========================================================================================================================*/
long Read_Dat(char *File_Name,short Sentence_Num,float **Feature,short Feature_Order)
{
	FILE *Fp;
	Fp	= fopen(File_Name,"rb");

	if(Fp==NULL)
		return 0;
	
	long Off_Set;//每句话的起始地址
	long FrameAmount;//每句话的帧数

	fseek(Fp,200,SEEK_SET);//索引区从200开始
	fseek(Fp,2*sizeof(long)*Sentence_Num,SEEK_CUR);//到达该句子对应的索引处
	fread(&Off_Set,sizeof(long),1,Fp);
	fread(&FrameAmount,sizeof(long),1,Fp);
	fseek(Fp,Off_Set,SEEK_SET);

	for(short iFrameNum=0;iFrameNum<FrameAmount;iFrameNum++)
	{
		fread(Feature[iFrameNum],sizeof(float),Feature_Order,Fp);
	}


	fclose(Fp);
	return FrameAmount;
}

/*========================================================================================================================

		函数名称：	Read_Wave_Dat
		函数功能：	从，dat文件中读出一句话的采样点
		函数性质：	公共函数
		输入参数：	char *File_Name	文件名
					short Sentence_Num	第几句话
		输出参数：	short *Wave_Dat	该句的采样
		返回值：	该句话的字节数
		备注：		

		基本思路：		
========================================================================================================================*/
long Read_Wave_Dat(char *File_Name,short Sentence_Num,short *Wave_Dat)
{
	FILE *Fp;
	Fp	= fopen(File_Name,"rb");

	if(Fp==NULL)
		return 0;
	
	long Off_Set;//每句话的起始地址
	long SentenceLength_Byte;//每句话的字节数

	fseek(Fp,200,SEEK_SET);//索引区从200开始
	fseek(Fp,2*sizeof(long)*Sentence_Num,SEEK_CUR);
	fread(&Off_Set,sizeof(long),1,Fp);
	fread(&SentenceLength_Byte,sizeof(long),1,Fp);
	fseek(Fp,Off_Set,SEEK_SET);

    fread(Wave_Dat,SentenceLength_Byte/2,sizeof(short),Fp);

	fclose(Fp);
	return SentenceLength_Byte/2;
}

/*========================================================================================================================
		函数名称：	Read_Sentence_Amount
		函数功能：	从，dat文件中读出共有多少句话
		函数性质：	公共函数
		输入参数：	char *File_Name	文件名
		输出参数：	无
		返回值：	句子数
		备注：		

		基本思路：		
========================================================================================================================*/
short Read_Sentence_Amount(char *File_Name)
{
	short Sentence_Amount;
	FILE *Fp;
	Fp	= fopen(File_Name,"rb");
	if(Fp==NULL)
		return 0;

	//读入该特征文件中包含的句子总数
	fseek(Fp,198,SEEK_SET);
	fread(&Sentence_Amount,sizeof(short),1,Fp);

	fclose(Fp);
	return Sentence_Amount;
}