#include "Wav_File_Handle.h"
int main()
{
	FILE *fp;
	if ((fp = fopen("voice.wav", "rb")) == NULL) {
		cout << "ERROR : File open failed !" << endl;
		getchar();
		exit(-1);
	}
	Wav_File_Work temp(fp);
	getchar();
	return 0;
}