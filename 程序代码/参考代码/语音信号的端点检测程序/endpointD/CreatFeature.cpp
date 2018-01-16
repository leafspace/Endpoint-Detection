/*******************************************************************
****	文件：		CreatFeature.cpp			****
****	功能：		生成特征矩阵				****
**** 			 					****
****	修改记录：	2001.7.23 摘自G.732.1          		****
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "Head.h"
#include "Read_Dat.h"
#include "mfcc_xhg.h"



int ExtractFeature(char *filename_in, char *filename_VadOut);
//特征提取程序

void main(int argc,char **argv)
{
/*	argc = 3;
	char	sourcefilename[]	= "E:\\Program_Data\\temp_data\\zx.dat";
	char	 Vadfilename[]		= "E:\\Program_Data\\temp_data\\zxvad.dat";
//*/
	  
//*
	//if(argc!= 3) {
		//printf("Usage: endpointD <sourcefilename.dat> <endpoint.txt>\n");
		//exit(0);
	//}
  char sourcefilename[100],  endpointfilename[100];
  
  strcpy(sourcefilename,argv[1]);
  strcpy(endpointfilename, argv[2]);
//*/
  int returnflag;
  
  returnflag=ExtractFeature(sourcefilename, endpointfilename);
  
  printf("Finished !\n");
}



//函数名：ExtractFeature
//传入参数：输入语音文件名 filename_in,输出文件名filename_Cepout
//         添加到文件头中的额外信息
//返回是否成功的标志,0为失败，1为成功
//说明：提取LPCC特征数据函数

int ExtractFeature(char *filename_in, char *filename_Vadout)
{
    FILE *fp_in, *fp_Vadout;
	
	//与语音数据相关的变量
	short	testflag;			//为了跟踪使用，在现有程序中已经不使用了
	int		TotalNumSentence;	//总句子数
    int		SentenceCount;		//句子循环计数器
	long	SentenceLength_Byte;
	long	datahead; 
	long	lStartPnt, lEndPnt;
	long	lSampleRate=16;
    
	
	//与特征提取相关的变量
	short	*VoiceData_Short	= NULL;//16bit 

	dataHEAD filehead;
	
	//--------------------------------------
	//取系统日期
	char datebuff[128];
	_tzset();
	_strdate(datebuff);
	//--------------------------------------
	

    fp_in		= fopen(filename_in,"rb");
	fp_Vadout	= fopen(filename_Vadout, "w+");
	if((fp_in==NULL)||(fp_Vadout == NULL))
		return 0;

	fread(&filehead,sizeof(dataHEAD),1,fp_in);
	printf("Now the voice data come from file: %s\n",filename_in);
	printf("Sample Rate: %d    Quantity byte: %d \n", filehead.SampleRate, filehead.SampleType );
	printf("output: %s\n",filename_Vadout);
	
	TotalNumSentence=Read_Sentence_Amount(filename_in);
	
	fprintf(fp_Vadout, "TotalNumSentence= %d\n", TotalNumSentence);
    
	for(SentenceCount=0; SentenceCount<TotalNumSentence; SentenceCount++)
	{
		//-------------------------------------------------------------------------------------
		//得到每个句子的长度，填充到相应的SentenceLength_Byte,SentenceLength_Short
		//中去。为VoiceData_Short或VoiceData_Byte数据区分配内存
		//同时将数据读入到对应的VoiceData_Short或VoiceData_Byte数据区
		fseek(fp_in,200+SentenceCount*2*sizeof(long),SEEK_SET);
		fread(&datahead,sizeof(long),1,fp_in);
		fread(&SentenceLength_Byte,sizeof(long),1,fp_in);

		fseek(fp_in,datahead,SEEK_SET);
		VoiceData_Short=(short *)malloc(SentenceLength_Byte);	
		testflag=fread(VoiceData_Short,SentenceLength_Byte,1,fp_in);
	    
		StartEnd(VoiceData_Short, SentenceLength_Byte/2, 
			&lStartPnt, &lEndPnt,
			lSampleRate);

		//输出端点结果
		fprintf(fp_Vadout, "%ld\t%ld\n", lStartPnt / LPCCFIX_FRAME, lEndPnt / LPCCFIX_FRAME);

		//删除内存的分配
		if(VoiceData_Short!=NULL)
		{
			free(VoiceData_Short);
			VoiceData_Short=NULL;
		}

		//fprintf(stdout, "Done: %3d\r", SentenceCount);

	}

	//关闭文件
	printf("\n");
	fclose(fp_in);
	fclose(fp_Vadout);
  
	return 1;
}


