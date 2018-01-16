#if !defined(_16to8_H__INCLUDED_)
#define _16to8_H__INCLUDED_

//16to8.h
#define numFilter 180
typedef struct {
	float x[numFilter+1];
	float B[numFilter+1];
	long j;
}FSDEF;

void initialfilter(void);
void initial_sentence(void);
//FIRÂË²¨Æ÷º¯Êý
short Filter(short sample,float *B,float *x,short nFilterOrder);
//³éÈ¡º¯Êý
void downsample_16_to_8_one_sentence( short *data_16, short *data_8, long *j);


#endif //!defined(_16to8_H__INCLUDED_)
