# define PI             3.14
# define Stride         0.005
short interleaver[length] = { ........... };     //注：interleaver是个交织器，长度与参数length一致就行，里面是乱序排列，这个在matlab里用randperm实现就行。

//例如：short interleaver[10] = {7,1,9,6,3,2,5,8,4,0};

void awgn(float db, int length, float *n)
{
	float pdf[10000] = { 0 };
	float *amp;
	float delta = 0, amplitude = 0, sum = 0, temp1 = 0, temp2 = 0;
	int *num;
	int counter, counter1, counter2, counter3;
	int half_length;
	delta = 1 / (2 * pow(10, 0.1*db));                     // 计算噪声功率
	counter = 0;                                            // 通过概率密度计算幅度集合和对应的概率密度
	while (sum < (0.5 / Stride))
	{
		pdf[counter] = 1 / sqrt(2 * PI*delta)*exp(-0.5*pow(amplitude, 2) / delta);
		sum += pdf[counter];
		amplitude += Stride;
		counter++;
	}                                                      // 最后得到的counter是幅度集合的长度
	num = (int *)calloc(counter, sizeof(int));              // num每个幅度在序列中出现的次数,幅度放在amp中
	amp = (float *)calloc(counter, sizeof(float));
	amplitude = 0;
	sum = 0;
	temp1 = length*Stride;
	temp2 = 0;
	for (counter1 = 0; counter1 < counter; counter1++)
	{
		temp2 = temp1*pdf[counter1];
		if ((int)(temp2 + 0.5) > (int)temp2)
			*(num + counter1) = (int)(temp2 + 1);
		else
			*(num + counter1) = (int)temp2;
		sum += *(num + counter1);
		*(amp + counter1) = amplitude;
		amplitude += Stride;
	}
	temp1 = 0;
	half_length = length >> 1;
	if (sum < half_length)                                     // 使num的和恰好是length的一半
	{
		temp1 = half_length - sum;
		for (counter1 = 0; counter1 < temp1; counter1++)
			(*(num + counter - 1 - counter1))++;
	}
	else if (sum > half_length)
	{
		temp1 = sum - half_length;
		*num -= temp1;
	}
	counter3 = 0;
	for (counter1 = 0; counter1 < counter; counter1++)             // 往*n里填入幅度值
	{
		for (counter2 = 0; counter2 < *(num + counter1); counter2++)
		{
			*(n + counter3) = *(amp + counter1);
			*(n + length - 1 - counter3) = -*(amp + counter1);
			counter3++;
		}
	}
	for (counter1 = 0; counter1 < length; counter1++)
	{
		temp1 = *(n + counter1);
		*(n + counter1) = *(n + interleaver[counter1]);
		*(n + interleaver[counter1]) = temp1;
		for (counter2 = 0; counter2 < length - counter1 - 1; counter2++)
			if (interleaver[counter1 + counter2 + 1] == counter1)
				interleaver[counter1 + counter2 + 1] = interleaver[counter1];
	}
}