#include "Header.h"

int main()
{
	FILE *fp;
	if ((fp = fopen("E:\\程序\\团队项目\\个性化文本语音转换系统\\语音\\微软默认.wav", "rb")) == NULL) {
		cout << "ERROR : File open failed !" << endl;
	}
	Wav_File f1(fp);
	if (f1.Read_File() == false) {
		exit(-1);
	}
	f1.ShowData();
	return 0;
}

