#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/soundcard.h>

#define LENGTH 3
#define RATE 8000
#define SIZE 8
#define CHANNELS 1

int main(int argc, char *argv[])
{
	char speechData;
	int voice_fp = open("/dev/audio1", O_RDONLY);
    int play_fp = open("/dev/audio", O_WRONLY);
	if(voice_fp == -1 || play_fp == -1){
		printf("Error : file open fuiled !\n");
		return 0;
	}

    int arg = LENGTH;
    int status;
    char buf[LENGTH * RATE * SIZE / 8];
	/* 设置采样时的量化位数 */
    arg = SIZE;
    status = ioctl(voice_fp, SOUND_PCM_WRITE_BITS, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_BITS ioctl failed");
    if (arg != SIZE)
        perror("unable to set sample size");

    /* 设置采样时的声道数目 */
    arg = CHANNELS; 
    status = ioctl(voice_fp, SOUND_PCM_WRITE_CHANNELS, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
    if (arg != CHANNELS)
        perror("unable to set number of channels");

    /* 设置采样时的采样频率 */
    arg = RATE;
    status = ioctl(voice_fp, SOUND_PCM_WRITE_RATE, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_WRITE ioctl failed");


    arg = SIZE;
    status = ioctl(play_fp, SOUND_PCM_WRITE_BITS, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_BITS ioctl failed");
    if (arg != SIZE)
        perror("unable to set sample size");

    /* 设置采样时的声道数目 */
    arg = CHANNELS; 
    status = ioctl(play_fp, SOUND_PCM_WRITE_CHANNELS, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
    if (arg != CHANNELS)
        perror("unable to set number of channels");

    /* 设置采样时的采样频率 */
    arg = RATE;
    status = ioctl(play_fp, SOUND_PCM_WRITE_RATE, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_WRITE ioctl failed");



    /* 循环，直到按下Control-C */
    while (1) {
        printf("Say something:\n");
        status = read(voice_fp, buf, sizeof(buf)); /* 录音 */
        if (status != sizeof(buf))
            perror("read wrong number of bytes");
        printf("You said:\n");
        status = write(play_fp, buf, sizeof(buf)); /* 回放 */
        if (status != sizeof(buf))
            perror("wrote wrong number of bytes");
        /* 在继续录音前等待回放结束 */
        status = ioctl(voice_fp, SOUND_PCM_SYNC, 0); 
        if (status == -1)
            perror("SOUND_PCM_SYNC ioctl failed");
    }

	close(voice_fp);
	return 0;	
}