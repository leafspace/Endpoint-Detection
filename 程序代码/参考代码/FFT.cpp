void FFT(unsigned long & ulN, vector<complex<double> >& vecList)
{
	//得到幂数
	unsigned long ulPower = 0; //幂数
	unsigned long ulN1 = ulN - 1;
	while (ulN1 > 0)
	{
		ulPower++;
		ulN1 /= 2;

	}
	//反序
	bitset<sizeof(unsigned long)* 8> bsIndex; //二进制容器
	unsigned long ulIndex; //反转后的序号
	unsigned long ulK;
	for (unsigned long p = 0; p < ulN; p++)
	{
		ulIndex = 0;
		ulK = 1;
		bsIndex = bitset<sizeof(unsigned long)* 8>(p);
		for (unsigned long j = 0; j < ulPower; j++)
		{
			ulIndex += bsIndex.test(ulPower - j - 1) ? ulK : 0;
			ulK *= 2;
		}
		if (ulIndex > p)
		{
			complex<double> c = vecList[p];
			vecList[p] = vecList[ulIndex];
			vecList[ulIndex] = c;
		}
	}
	//计算旋转因子
	vector<complex<double> > vecW;
	for (unsigned long i = 0; i < ulN / 2; i++)
	{
		vecW.push_back(complex<double>(cos(2 * i * PI / ulN), -1 * sin(2 * i * PI / ulN)));
	}
	for (unsigned long m = 0; m < ulN / 2; m++)
	{
		cout << "\nvW[" << m << "]=" << vecW[m];
	}
	//计算FFT
	unsigned long ulGroupLength = 1; //段的长度
	unsigned long ulHalfLength = 0; //段长度的一半
	unsigned long ulGroupCount = 0; //段的数量
	complex<double> cw; //WH(x)
	complex<double> c1; //G(x) + WH(x)
	complex<double> c2; //G(x) - WH(x)
	for (unsigned long b = 0; b < ulPower; b++)
	{
		ulHalfLength = ulGroupLength;
		ulGroupLength *= 2;
		for (unsigned long j = 0; j < ulN; j += ulGroupLength)
		{
			for (unsigned long k = 0; k < ulHalfLength; k++)
			{
				cw = vecW[k * ulN / ulGroupLength] * vecList[j + k + ulHalfLength];
				c1 = vecList[j + k] + cw;
				c2 = vecList[j + k] - cw;
				vecList[j + k] = c1;
				vecList[j + k + ulHalfLength] = c2;
			}
		}
	}
}