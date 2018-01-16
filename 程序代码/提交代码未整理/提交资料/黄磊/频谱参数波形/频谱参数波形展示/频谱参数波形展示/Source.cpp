#include "Wav_File_Handle.h"

int main()
{

	FILE *fp;
	if ((fp = fopen("E:\\程序\\团队项目\\个性化文本语音转换系统\\语音\\微软默认.wav", "rb")) == NULL) {
		cout << "ERROR : File open failed !" << endl;
		exit(-1);
	}
	Wav_File_Work temp(fp);
	temp.ShowData();
	return 0;
}