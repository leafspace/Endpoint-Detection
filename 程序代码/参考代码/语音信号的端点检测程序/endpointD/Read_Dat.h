//	函数功能：	从，dat文件中读出共有多少句话
short Read_Sentence_Amount(char *File_Name);
//	函数功能：	从，dat文件中读出一句话的采样点
long Read_Wave_Dat(char *File_Name,short Sentence_Num,short *Wave_Dat);

//		函数功能：	从，dat文件中读出一句话的特征
long Read_Dat(char *File_Name,short Sentence_Num,float **Feature,short Feature_Order);
//		函数功能：	从，dat文件中读出一句话的特征帧数或者是字节数
long Read_Length(char *File_Name,short Sentence_Num);