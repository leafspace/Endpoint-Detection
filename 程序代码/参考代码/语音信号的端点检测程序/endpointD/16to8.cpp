//16to8.cpp
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "16to8.h"

FSDEF fs;

void initialfilter(void)
{
float B[numFilter+1] ={
(float)0.000200 ,
(float)-0.000150,
(float)-0.000279,
(float)0.000024 ,
(float)0.000309 ,
(float)0.000125 ,
(float)-0.000276,
(float)-0.000273,
(float)0.000172 ,
(float)0.000392 ,
(float)0.000000 ,
(float)-0.000445,
(float)-0.000222,
(float)0.000398 ,
(float)0.000454 ,
(float)-0.000230,
(float)-0.000637,
(float)-0.000054,
(float)0.000706 ,
(float)0.000416 ,
(float)-0.000603,
(float)-0.000779,
(float)0.000302 ,
(float)0.001044 ,
(float)0.000175 ,
(float)-0.001105,
(float)-0.000751,
(float)0.000885 ,
(float)0.001294 ,
(float)-0.000361,
(float)-0.001645,
(float)-0.000408,
(float)0.001655 ,
(float)0.001280 ,
(float)-0.001229,
(float)-0.002046,
(float)0.000367 ,
(float)0.002474 ,
(float)0.000811 ,
(float)-0.002365,
(float)-0.002071,
(float)0.001616 ,
(float)0.003103 ,
(float)-0.000270,
(float)-0.003581,
(float)-0.001462,
(float)0.003255 ,
(float)0.003220 ,
(float)-0.002024,
(float)-0.004560,
(float)0.000000 ,
(float)0.005045 ,
(float)0.002477 ,
(float)-0.004362,
(float)-0.004880,
(float)0.002427 ,
(float)0.006587 ,
(float)0.000550 ,
(float)-0.007017,
(float)-0.004056,
(float)0.005777 ,
(float)0.007335 ,
(float)-0.002801,
(float)-0.009525,
(float)-0.001577,
(float)0.009831 ,
(float)0.006610 ,
(float)-0.007726,
(float)-0.011228,
(float)0.003121 ,
(float)0.014203 ,
(float)0.003530 ,
(float)-0.014373,
(float)-0.011205,
(float)0.010877 ,
(float)0.018398 ,
(float)-0.003365,
(float)-0.023275,
(float)-0.007871,
(float)0.023848 ,
(float)0.021885 ,
(float)-0.018065,
(float)-0.037169,
(float)0.003519 ,
(float)0.051875 ,
(float)0.024193 ,
(float)-0.064095,
(float)-0.080483,
(float)0.072178 ,
(float)0.309445 ,
(float)0.425024 ,
(float)0.309445 ,
(float)0.072178 ,
(float)-0.080483,
(float)-0.064095,
(float)0.024193 ,
(float)0.051875 ,
(float)0.003519 ,
(float)-0.037169,
(float)-0.018065,
(float)0.021885 ,
(float)0.023848 ,
(float)-0.007871,
(float)-0.023275,
(float)-0.003365,
(float)0.018398 ,
(float)0.010877 ,
(float)-0.011205,
(float)-0.014373,
(float)0.003530 ,
(float)0.014203 ,
(float)0.003121 ,
(float)-0.011228,
(float)-0.007726,
(float)0.006610 ,
(float)0.009831 ,
(float)-0.001577,
(float)-0.009525,
(float)-0.002801,
(float)0.007335 ,
(float)0.005777 ,
(float)-0.004056,
(float)-0.007017,
(float)0.000550 ,
(float)0.006587 ,
(float)0.002427 ,
(float)-0.004880,
(float)-0.004362,
(float)0.002477 ,
(float)0.005045 ,
(float)0.000000 ,
(float)-0.004560,
(float)-0.002024,
(float)0.003220 ,
(float)0.003255 ,
(float)-0.001462,
(float)-0.003581,
(float)-0.000270,
(float)0.003103 ,
(float)0.001616 ,
(float)-0.002071,
(float)-0.002365,
(float)0.000811 ,
(float)0.002474 ,
(float)0.000367 ,
(float)-0.002046,
(float)-0.001229,
(float)0.001280 ,
(float)0.001655 ,
(float)-0.000408,
(float)-0.001645,
(float)-0.000361,
(float)0.001294 ,
(float)0.000885 ,
(float)-0.000751,
(float)-0.001105,
(float)0.000175 ,
(float)0.001044 ,
(float)0.000302 ,
(float)-0.000779,
(float)-0.000603,
(float)0.000416 ,
(float)0.000706 ,
(float)-0.000054,
(float)-0.000637,
(float)-0.000230,
(float)0.000454 ,
(float)0.000398 ,
(float)-0.000222,
(float)-0.000445,
(float)0.000000 ,
(float)0.000392 ,
(float)0.000172 ,
(float)-0.000273,
(float)-0.000276,
(float)0.000125 ,
(float)0.000309 ,
(float)0.000024 ,
(float)-0.000279,
(float)-0.000150,
(float)0.000200 ,
};
	memset(&fs.x, 0, sizeof(fs.x));
	for (int i=0; i<numFilter+1; i++)
		fs.B[i] = B[i];
}

void initial_sentence(void)
{
	fs.j=0;
}

/*************************************************************
	FIR滤波器函数
	sample = 输入待滤波的数
	B --> 滤波器参数
	x --> 滤波器中间状态
	nFilterOrder = 滤波器阶数
*************************************************************/
short Filter(short sample,float *B,float *x,short nFilterOrder)
{
	short	i;
	float	y;
	float   z;
	
	short rtn;

	for( i=nFilterOrder; i>0; i-- )
	{
		x[i]=x[i-1];
	}

	x[0] = (float)sample;
	y = (float)0.0;

	for( i=0; i<=nFilterOrder; i++ )
		y += (B[i] * x[i] );

	z=(float)floor((double)y);//取整
	rtn=(short)z;//截短

	return rtn;
}
/*************************************************************
	downsample_16_to_8
	抽取函数
	*data_16					16k采样的数据头
	*data_8						8k采样的数据头
	*j							采样计数
*************************************************************/
void downsample_16_to_8_one_sentence( short *data_16, short *data_8, long *j)
{
	long i;
	i=*j;
	data_8[i]=data_16[2*i];
	(*j)++;
}

