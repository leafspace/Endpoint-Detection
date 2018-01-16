#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/soundcard.h>

int main(int argc, char *argv[])
{
	char speechData;
	int voice_fp = open("/dev/audio1", O_RDONLY);
	if(voice_fp == -1){
		voice_fp = open("/dev/audio", O_RDONLY);
		if(voice_fp == -1){
			printf("Error : file open fuiled !\n");
			return 0;
		}else{
			printf("info : this driver audio. \n");
		}
	}else{
		printf("info : this driver audio1. \n");
	}

	int format_t;
    int format[7];
    format[0] = AFMT_S16_LE;
    format[1] = AFMT_U8;        //unsigned 8b
    format[2] = AFMT_S8;        //signed   8b
    format[3] = AFMT_U16_LE;    //unsigned 16b small
    format[4] = AFMT_U16_BE;    //unsigned 16b big
    format[5] = AFMT_MPEG;
    format[6] = AFMT_AC3;
    for(int i = 0; i < 7; ++i){
    	format_t = format[i];
    	if (ioctl(voice_fp, SNDCTL_DSP_GETFMTS, &format_t) == -1) {
    	}else{
    		switch(i)
    		{
    			case 0: printf("info : this driver support AFMT_S16_LE. \n"); break;
    			case 1: printf("info : this driver support AFMT_U8. \n"); break;
    			case 2: printf("info : this driver support AFMT_S8. \n"); break;
    			case 3: printf("info : this driver support AFMT_U16_LE. \n"); break;
    			case 4: printf("info : this driver support AFMT_U16_BE. \n"); break;
    			case 5: printf("info : this driver support AFMT_MPEG. \n"); break;
    			case 6: printf("info : this driver support AFMT_AC3. \n"); break;
    			default: printf("Error : format error !\n"); break;
    		}
    	}
    }
	close(voice_fp);
	return 0;	
}
