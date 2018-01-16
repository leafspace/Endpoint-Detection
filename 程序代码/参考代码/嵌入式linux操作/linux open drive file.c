1) 头文件定义

/** Standard includes*/
#include <ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>

/** Mandatory variables.*/
#define BUF_SIZE 4096
int audio_fd;
unsigned char audio_buffer[BUF_SIZE];



1. 打开音频设备

if ((audio_fd = open(DEVICE_NAME, open_mode, 0)) == -1) {
    /* Open of device failed */
    perror(DEVICE_NAME);
    exit(1);
}

//O_RDONLY，O_WRONLY，O_RDWR | 只读，只写，读写


2. 录音

int len;
if ((len = read(audio_fd, audio_buffer, count)) == -1) {
    perror("audio read");
    exit(1);
}

3. 放音

int len;
if ((len = write(audio_fd, audio_buffer, count)) == -1) {
    perror("audio read");
    exit(1);
}

4. 设置参数

    设置采样格式
    
    int format;
    format = AFMT_S16_LE;
    if (ioctl(audio_fd, SNDCTL_DSP_SETFMT, &format) == -1) {
        /* fatal error */
        perror("SNDCTL_DSP_SETFMT");
        exit(1);
    }
    if (format != AFMT_S16_LE) {
        /* 本设备不支持选择的采样格式. */
    }

    在设置采样格式之前，可以先测试设备能够支持那些采样格式，方法如下：

    采样格式包括AFMT_U8（无符号8位）、AFMT_S8（有符号8位）、AFMT_U16_LE（小端模式，无符号16位）、 AFMT_U16_BE（大端模式，无符号16位）、AFMT_MPEG、AFMT_AC3
    int mask;
    if (ioctl(audio_fd, SNDCTL_DSP_GETFMTS, &mask) == -1) {
        /* Handle fatal error ... */
    }
    if (mask & AFMT_MPEG) {
        /* 本设备支持MPEG采样格式 ... */
    }
    
    设置通道数目
    int channels = 2;
    /* 1=mono, 2=stereo */
    if (ioctl(audio_fd, SNDCTL_DSP_CHANNELS, &channels) == -1) {
        /* Fatal error */
        perror("SNDCTL_DSP_CHANNELS");
        exit(1);
    }
    if (channels != 2){
        /* 本设备不支持立体声模式 ... */
    }
    
    设置采样速率
    
    int speed = 11025;
    if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &speed)==-1) {
        /* Fatal error */
        perror("SNDCTL_DSP_SPEED");
        exit(Error code);
    }
    if ( /* 返回的速率（即硬件支持的速率）与需要的速率差别很大... */ ) {
        /* 本设备不支持需要的速率... */
    }

    调节音量

    int vol;
    if (ioctl(mixer_fd, SOUND_MIXER_READ(SOUND_MIXER_MIC), &vol) == -1) {
        /* 访问了没有定义的mixer通道... */
    }




例子
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/soundcard.h>
#define LENGTH 3    /* 存储秒数 */
#define RATE 8000   /* 采样频率 */
#define SIZE 8      /* 量化位数 */
#define CHANNELS 1  /* 声道数目 */
/* 用于保存数字音频数据的内存缓冲区 */
unsigned char buf[LENGTH*RATE*SIZE*CHANNELS/8];
int main()
{
    int fd;	/* 声音设备的文件描述符 */
    int arg;	/* 用于ioctl调用的参数 */
    int status;   /* 系统调用的返回值 */
    /* 打开声音设备 */
    fd = open("/dev/dsp", O_RDWR);
    if (fd < 0) {
        perror("open of /dev/dsp failed");
        exit(1);
    }
    /* 设置采样时的量化位数 */
    arg = SIZE;
    status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_BITS ioctl failed");
    if (arg != SIZE)
        perror("unable to set sample size");
    /* 设置采样时的声道数目 */
    arg = CHANNELS; 
    status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
    if (arg != CHANNELS)
        perror("unable to set number of channels");
    /* 设置采样时的采样频率 */
    arg = RATE;
    status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
    if (status == -1)
        perror("SOUND_PCM_WRITE_WRITE ioctl failed");
    /* 循环，直到按下Control-C */
    while (1) {
        printf("Say something:/n");
        status = read(fd, buf, sizeof(buf)); /* 录音 */
        if (status != sizeof(buf))
            perror("read wrong number of bytes");
        printf("You said:/n");
        status = write(fd, buf, sizeof(buf)); /* 回放 */
        if (status != sizeof(buf))
            perror("wrote wrong number of bytes");
        /* 在继续录音前等待回放结束 */
        status = ioctl(fd, SOUND_PCM_SYNC, 0); 
        if (status == -1)
            perror("SOUND_PCM_SYNC ioctl failed");
  }
}