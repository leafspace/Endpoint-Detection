#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	char speechData;
	int voice_fp = open("/dev/audio1", O_RDONLY);
	if(voice_fp == -1){
		printf("Error : file open fuiled !\n");
	}else{
		while(true){
			read(voice_fp, &speechData, 1);
			int data_t = (int)speechData;
			printf("%d\n", data_t);
		}
	}
	close(voice_fp);
	return 0;	
}