#ifndef VOICE_H
#define VOICE_H

#include <QVector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/soundcard.h>

#define SIZE 16
#define RATE 16000
#define CHANNELS 1
#define BUFSIZE 1600
#define GRAPH_SPEED 100

typedef short ElemType;                                                        //随着SIZE的变化而变化  /* SIZE       type 8          char 16         short 32         int 64         long */
typedef struct Wav_file
{
    char id_RIFF[4];                                                           //RIFF块标志
    int fileSize;                                                              //文件的总字节数
    char id_WAVE[4];                                                           //WAV标志

    char id_FMT[4];                                                            //格式块标志
	int formatLength;                                                          //格式块长度，16为正常，18说明有附加信息
	short int formatTag;                                                       //格式类别，值=1 表示编码方式为PCMu律编码
	short int channelsNumber;                                                  //声道数
	int sampleRate;                                                            //每秒的样本数
	int secondBytes;                                                           //每秒数据的字节数
	short int chunkAlign;                                                      //采样字节数
	short int sampleBits;                                                      //采样位数

	char id_DATA[5];                                                           //附加块标志
	int dataSize;                                                              //数据部分字节数
}Wav_file;

extern int paragraph_voice;
extern QVector<ElemType*> speech_voice;

bool OpenDrive(int *in);
bool OpenDrive(int *in, int *out);

int SetDrive(int *in);
int SetDrive(int *in, int *out);

#endif // VOICE_H

