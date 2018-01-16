class Wav_File_Work :protected Wav_File
{
private:
	double *New_dataDouble;													 //用于保存快速傅立叶变换之前补零后数据
	double *dataFFT;														 //用于保存快速傅立叶变换后数据
	double *Em;															     //用来保存频谱能量
	double *SCm;														     //用来保存频谱质心
	double *BWm;															 //用于保存宽带
	double **SERmi;															 //用于保存子带能量比
	double *Nor_dataDouble;													 //用于保存归一化数据
	double **SFmi;															 //用于保存子带谱流量
	double *RPm;															 //用于保存频谱滚降点
	double **Ai;															 //线性预测系数
	double *err;															 //增益
	double *Xn;																 //用于保存短时傅立叶变换后数据
	double *H;																 //线性预测谱
	double **h;																 //线性预测倒谱系数
	double *h_Spectrum;										                 //线性预测倒谱

	int Is_Pow2(void);														 //判断FFT长度是否为2的整数次方
	long Add_Zero(int i);													 //补零
	void Reverse_dataDouble(double *data, int len);						     //乱序输入数据
	int FFT(void);															 //快速傅立叶变换

	int spectralEnergy(int len);											 //计算频谱能量
	int spectralCentroid(int len);										     //频谱质心
	int bandWidth(int len);													 //宽带
	int Subband_Energy_Ratio(int len);										 //子带能量比
	void Normalized_Spectral_Signal(int k);								     //归一化频谱信号
	int Subband_Spectral_Flux(int len);									     //子带谱流量
	int Spectral_Rolloff_Point(int len);								     //频谱滚降点
	int LPC_Levinson_Durbin(void);											 //线性预测系数
	int STFT(void);															 //短时傅立叶变换
	int LPC(void);															 //线性预测谱
	int LPCC_Coefficien(void);												 //线性预测倒谱系数
	int LPCC(void);                                                          //线性预测倒谱

public:
	static const int Sub_band;												 //子带个数3
	static const int P;														 //LPC阶数
};


const int Wav_File_Work::Sub_band = 16;										 //子带个数
const int Wav_File_Work:: P=14;												 //LPC阶数


/**********************************************************************
*        用来判断进行快速傅立叶变换的数据个数是否为2的整次方              *
*		 如果是，则返回0，否则返回个数最接近2的整次幂的幂数               *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::Is_Pow2(void)
{
	double i;
	double f = Wav_File::Get_dataNumber();
	int j;																	 //存放数据个数的2的整次幂
	for (j = 1; (f = f / 2) != 1 && f>1; j++);
	if (pow(2, j) - Wav_File::Get_dataNumber() == 0)
		return 0;
	else
		return j;
}


/**********************************************************************
*              用于数据个数不足2的整次幂时，将空缺位置补零               *
*                  int  i  :	2的整次幂的幂数                        *
*				     函数返回补零后数据的总个数						  *
*                          作者：詹太安                                *
**********************************************************************/
long Wav_File_Work::Add_Zero(int i)
{
	for (int j = 0; j < Wav_File::Get_dataNumber(); j++)					 //将原数组数据赋值给补零数组
		New_dataDouble[j] = dataDouble[j];
	for (int j= Wav_File::Get_dataNumber(); j < pow(2, i); j++)			     //在原数组数据上补零
		New_dataDouble[j] = 0;
	return pow(2, i);
}


/**********************************************************************
*      用于打乱进行快速傅立叶变换的数据顺序，以达到最后的输出是顺序的      *
* struct compx *s : 以结构体形式存储的输入数据，快速傅立叶变换的结果同样保存在此结构体中 *
*                   int len : 数据的总个数                            *
*                          作者：詹太安                                *
**********************************************************************/
void Wav_File_Work::Reverse_dataDouble(double *data, int len)				 //雷德算法乱序输入数据
{

	long i, k, j = 0;
	double t;
	long nv2 = len / 2;
	long nm1 = len;
	for (i = 0; i < nm1 - 1; i++)
	{
		if (i < j)															 //进行数据交换
		{
			t = data[j];
			data[j] = data[i];
			data[i] = t;
		}
		k = nv2;
		while (k <= j)
		{
			j = j - k;
			k = k / 2;
		}
		j = j + k;
	}
}

/**********************************************************************
*                       进行快速傅立叶变换                             *
*					返回快速傅立叶变换后的数据个数                      *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::FFT(void)
{

	int l;																	 //存放幂数
	long len;																 //存放数据长度
	if (Is_Pow2())
	{
		New_dataDouble = (double*)malloc(sizeof(double)*pow(2, Is_Pow2()));  //为新的数组申请空间，空间大小为补零之后的空间长度
		len = Add_Zero(Is_Pow2());											 //长度为补零后的数组长度
	}
	else
	{
		New_dataDouble = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber());//为新的数组申请空间，空间大小为原数组空间长度
		New_dataDouble = dataDouble;										 //将原数组数据赋值给新数组
		len = Wav_File::Get_dataNumber();								     //长度为原数组长度
	}
		
	int f = len;
	dataFFT = (double *)malloc(sizeof(double)*len);							 //为快速傅立叶变换后数据申请空间
	if (dataFFT == NULL) {													 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (int i = 0; i < len; i++)
	{
		dataFFT[i] = New_dataDouble[i];										 //初始化快速傅立叶变换数据
	}

	for (l = 1; (f = f / 2) != 1; l++);										 //得到新数组总长度幂数
	Reverse_dataDouble(dataFFT, len);										 //乱序排列

	double *W = (double *)malloc(sizeof(double)*len/2);						 //计算旋转因子
	for (unsigned long i = 0; i < len / 2; i++)
	{
		W[i] = cos(2 * i * PI / len);
	}

	unsigned long ulGroupLength = 1;										 //段的长度
	unsigned long ulHalfLength = 0;											 //段长度的一半
	unsigned long ulGroupCount = 0;											 //段的数量
	double cw, c1, c2;
	for (unsigned long b = 0; b < l; b++)									 //计算FFT
	{
		ulHalfLength = ulGroupLength;
		ulGroupLength *= 2;
		for (unsigned long j = 0; j < len; j += ulGroupLength)
		{
			for (unsigned long k = 0; k < ulHalfLength; k++)
			{
				cw = W[k * len / ulGroupLength], dataFFT[j + k + ulHalfLength];
				c1 = dataFFT[j + k] + cw;
				c2 = dataFFT[j + k] - cw;
				dataFFT[j + k] = c1;
				dataFFT[j + k + ulHalfLength] = c2;
			}
		}
	}
	for (int i = 0; i < len; i++)
		cout << i << " : " << dataFFT[i] << " ";
	return len;																 //返回快速傅立叶变换数据的长度
}


/**********************************************************************
*                       计算频谱能量                                   *
*             int  len  :计算频谱能量数据的个数                          *
*				频谱能量保存在数组 double *Em 中                        *
*				        返回频谱能量的个数                             *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::spectralEnergy(int len)
{
	int j, k;
	Em = (double*)malloc(sizeof(double)*(len - N));							 //为频谱能量申请内存空间
	if (Em == NULL) {														 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (k = 0, j = 0; k < len - N; k += Wav_File_Work::FrameShift, j++)
	{
		Em[j] = 0;
		for (int i = k; i < k + N; i++)
		{
			//Em[j]+=(dataFFT[i]*dataFFT[i])*Hamming_window(k + N - 1 - i);	 //计算快速傅立叶变换后的数据的频谱能量
			Em[j]+=(dataDFT[i]*dataDFT[i])*Hamming_window(k + N - 1 - i);	 //计算离散傅立叶变换后的数据的频谱能量
		}
	}
	return j;																 //返回频谱能量个数
}


/**********************************************************************
*						 计算频谱质心						             *
*				频谱质心保存在数组 double *SCm 中                       *
*				int  len :计算频谱质心数据的个数                        *
*					    返回频谱质心个数							   *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::spectralCentroid(int len)
{
	int j, k;
	SCm = (double*)malloc(sizeof(double)*(len - N));						 //为频谱质心申请内存空间
	if (SCm == NULL) {														 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (k = 0, j = 0; k < len - N; k += Wav_File_Work::FrameShift, j++)
	{
		SCm[j] = 0;
		for (int i = k; i < k + N; i++)
		{
			//SCm[j]+=(dataFFT[i]*dataFFT[i])*Hamming_window(k + N - 1 - i)*i;//计算快速傅立叶变换后的数据的频谱质心
			SCm[j]+=(dataDFT[i]*dataDFT[i])*Hamming_window(k + N - 1 - i)*i; //计算离散傅立叶变换后的数据的频谱质心
		}
		SCm[j] = SCm[j] / Em[j];
	}
	return j;																 //返回频谱质心个数
}


/**********************************************************************
*                           计算带宽                                  *
*					int  len :计算带宽数据的个数						 *
*					带宽保存在数组 double *BWm 中                     *
*							返回带宽个数								 *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::bandWidth(int len)
{
	int j, k;
	BWm = (double*)malloc(sizeof(double)*(len - N));						 //为宽带申请内存空间
	if (BWm == NULL) {														 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (k = 0, j = 0; k < len - N; k += Wav_File_Work::FrameShift, j++)
	{
		double sum = 0;
		for (int i = k; i < k + N; i++)
		{
			//sum+=pow((i-SCm[j]),2)*(dataFFT[i]*dataFFT[i])*Hamming_window(k + N - 1 - i);//计算快速傅立叶变换后的数据的宽带
			sum+=pow((i-SCm[j]),2)*(dataDFT[i]*dataDFT[i])*Hamming_window(k + N - 1 - i);//计算离散傅立叶变换后的数据的宽带
		}
		BWm[j] = sqrt(sum / Em[j]);
	}
	
	return j;																 //返回宽带个数
}


/**********************************************************************
*                         计算子带能量比                               *
*				int  len :计算子带能量比的数据的个数					   *
*				子带能量比保存在数组 double **SERmi 中                  *
*							返回子带能量比个数						  *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::Subband_Energy_Ratio(int len)
{
	int i, n;
	double f = 0;
	SERmi = (double**)malloc(sizeof(double)*(len - N));						 //为子带能量比申请内存空间
	if (SERmi == NULL) {													 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < len - N; i += Wav_File_Work::FrameShift, n++)
	{
		SERmi[n] = (double*)malloc(Sub_band*sizeof(double));
		double sum = 0;
		double p = N / Sub_band;											 //Sun_band为划分的子带个数
		if ((int)p != p)
		{
			p = (int)p + 1;
		}
		f = p;
		int j = i, k = 0;
		for (; k < 15; k++)
		{
			for (; j <p + i; j++)
			{
				//sum += (dataFFT[i]*dataFFT[i])*Hamming_window(i + N - 1 - j);//计算快速傅立叶变换后的数据的子带能量比
				sum += (dataDFT[i]*dataDFT[i])*Hamming_window(i + N - 1 - j);//计算离散傅立叶变换后的数据的子带能量比
			}
			SERmi[n][k] = sum / Em[n];
			p += f;
			if (p>len)
				p = len;
		}
	}
	return n;																 //返回子带能量比个数
}


/**********************************************************************
*                        将频谱信号归一化                              *
*				 int  k ： 归一化的数据下标                            *
* 归一化的频谱信号保存在新的数组 double *Nor_real和 double *Nor_imag 中 *
*			*Nor_real 存放实部数据，*Nor_imag 存放虚部数据		      *
*                          作者：詹太安                                *
**********************************************************************/
void Wav_File_Work::Normalized_Spectral_Signal(int k)
{
	double sum = 0;
	for (int n = k; n < k + N; n++)
	{
		//Nor_dataDouble[n] = 10 * log(dataFFT[n])/log(10);					 //快速傅立叶变换之后频谱信号的归一化
		Nor_dataDouble[n] = 10 * log(dataDFT[n] * dataDFT[n]) / log(10);	 //离散傅立叶变换之后频谱信号的归一化
		sum += (Nor_dataDouble[n] * Nor_dataDouble[n]) * Hamming_window(k + N - 1 - n);

	}
	Nor_dataDouble[k] = Nor_dataDouble[k] / sqrt(sum);
}


/**********************************************************************
*							计算子带谱流量							  *
*					int  len：计算子带谱流量的数据的个数                *
*					子带谱流量保存在 double **SFmi 中                  *
*							返回子带谱流量个数                         *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::Subband_Spectral_Flux(int len)
{
	int i, n;
	double f = 0;
	Nor_dataDouble = (double*)malloc(sizeof(double)*(len - N));				 //为归一化实数部分申请内存空间
	if (Nor_dataDouble == NULL) {											 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	SFmi = (double**)malloc(sizeof(double)*(len - N));						 //为子带谱流量申请内存空间
	if (SFmi == NULL) {														 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i <len - N; i += Wav_File_Work::FrameShift, n++)
	{
		SFmi[n] = (double*)malloc(Sub_band*sizeof(double));
		
		double p = N / Sub_band;											 //Sun_band为划分的子带个数
		if ((int)p != p)
		{
			p = (int)p + 1;
		}
		f = p;
		int j = i, k = 0;
		for (; k < 15; k++)
		{
			double sum = 0;
			int l = j;
			for (; j <p + i; j++)											 //j表示从每一个子带的频率上限到下限的循环变量
			{
				Normalized_Spectral_Signal(j);
				Normalized_Spectral_Signal(j + 1);
				sum += (Nor_dataDouble [j + 1] - Nor_dataDouble[j])*Hamming_window(i + N - 1 - j);
			}
			SFmi[n][k] = sum / (p + i - l);
			p += f;
			if (p>len)
				p = len;
		}
	}
	return n;																 //返回子带谱流量个数
}


/**********************************************************************
*							计算频谱滚降点                             * 
*					int  len :计算频谱滚降点的数据的个数				  *	
*                          滚降点计算不出来                            * 
*                          作者：詹太安                                *
**********************************************************************/
/*int Wav_File_Work::Spectral_Rolloff_Point(int len)																					
{
	double sum1, sum2;
	int i, j;
	RPm = (double*)malloc(sizeof(double)*(len - N));						 //为频谱滚降点申请内存空间
	if (RPm == NULL) {														 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0,j=0; i < len-N ; i+=Wav_File_Work::FrameShift,j++)
	{
		sum1 = 0, sum2 = 0;
		
		for (int n = i; n < i + N; n++)
		{
			sum1 += dataDFT[n];
		}
		for (int n = i; n < i + N; n++)
		{
			sum2 +=real[n];
			if (sum2 == 0.85*sum1)
			{
				RPm[j] = n;
				break;
			}
		}
	}
	return j;																 //返回频谱滚降点个数
}*/																			 //詹太安


/**********************************************************************
*      计算线性预测系数即LPC系数 以及增益G即预测误差E的平方根             *
* 线性预测系数保存在数组 double **Ai 中，增益G保存在数组 double *err 中  *
*						返回LPC系数个数								  *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::LPC_Levinson_Durbin(void)
{
	double E = 0;															 //预测误差
	double **r = (double **)malloc(sizeof(double)*(Wav_File::Get_dataNumber() - N));//相关系数
	Ai = (double**)malloc(sizeof(double)*(Wav_File::Get_dataNumber() - N));	 //为线性预测系数申请内存空间
	if (Ai == NULL) {														 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	err = (double *)malloc(sizeof(double)*(Wav_File::Get_dataNumber() - N));
	if (err == NULL) {														 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	double *p_tmp = (double *)malloc(sizeof(double)*(P + 1));
	double *p_buf = (double *)malloc(sizeof(double)*(P + 1));
	int i, n;
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)	//信号加窗,分帧,计算相关系数
	{
		r[n] = (double*)malloc(sizeof(double)*(P + 1));
		for (int m = 0; m <= P; m++)
		{
			double sum = 0;
			for (int k = i, j = m + i; j < i + N; k++, j++)
			{
				sum += dataDouble[k] * dataDouble[j] * Hamming_window(i + N - 1 - k);
				r[n][m] = sum;
			}
		}
	}
	int j, m, k;
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++) //计算线性预测系数以及增益
	{
		Ai[n] = (double*)malloc(sizeof(double)*(P + 1));
		if (0 == r[n][0])
		{
			for (m = 0; m <= P; m++)
				Ai[n][m] = 0;
			return 0;
		}
		for (int m = 1; m <= P; m++)
		{
			Ai[n][m] = 0;
			p_buf[m] = 0;
		}
		Ai[n][0] = 1.0;
		p_buf[0] = 1;
		err[n] = r[n][0];
		for (m = 0; m <= P; m++)
		{
			double lambda = 0.0;
			for (j = 0; j < m; j++)
			{
				k = m - j;
				if (k < 0)
					k = -k;
				lambda -= Ai[n][j] * r[n][k];
			}
			lambda /= err[n];
			for (j = 1; j <= m; j++)
			{
				p_buf[j] = Ai[n][j] + lambda * Ai[n][m - j];
			}
			err[n] *= (1.0 - lambda*lambda);
			p_tmp = Ai[n];
			Ai[n] = p_buf;
			p_buf = p_tmp;
		}
		err[n] = sqrt(abs(err[n]));											 //增益G
	}
	return n;																 //返回线性预测系数个数
}


/**********************************************************************
*                       进行短时傅立叶变换                             *
*			短时傅立叶变换结果保存在结构体struct compx *Xn 中			 *
*					返回短时傅立叶变换后的数据个数			              *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::STFT(void)
{
	int i, n;
	Xn= (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber()-N);		 //为短时傅立叶变换数据申请内存空间
	if (Xn == NULL) {														 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)
	{
		Xn[n] = 0;
		for (int k = i; k < i + N; k++)
		{
			Xn[n]+= dataDouble[k] * Hamming_window(i + N - 1 - k)*cos(2 * PI*k*i / N);
		}
	}
	return n;																 //返回短时傅立叶变换数据个数
}


/**********************************************************************
*                利用线性预测系数计算线性预测谱                         *
*				 LPC谱为复数谱，也称为LPC复谱			 		          *
*              线性预测谱保存在结构体 struct compx *H 中               *
*							返回LPC谱个数							  *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::LPC(void)
{
	int i, n;
	H = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber() - N);		 //为线性预测谱申请内存空间
	if (H == NULL) {														 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)
	{
		H[n] = 0;
		for (int j = 1; j <= P; j++)
		{
			H[n] += Ai[n][j] * cos(2 * PI*i*j / P);
		}
		H[n] = 1 - H[n];
		H[n] = err[n] / H[n];
	}
	return n;																 //返回线性预测谱个数
}


/**********************************************************************
*           利用线性预测系数计算线性预测倒谱系数——LPCC系数               *
*					LPCC系数保存在数组 double **h 中                   *
*							返回LPCC系数个数							  *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::LPCC_Coefficien(void)
{
	int i, j, n;
	double **LPCEP = (double **)malloc(sizeof(double)*(Wav_File::Get_dataNumber() - N));
	h = (double**)malloc(sizeof(double)*(Wav_File::Get_dataNumber() - N));	 //为线性预测倒谱系数申请内存空间
	if (h == NULL) {														 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)
	{
		LPCEP[n] = (double *)malloc(sizeof(double)*(P + 1));
		h[n] = (double *)malloc(sizeof(double)*(P + 1));
		for (j = 0; j <= P; j++)
			LPCEP[n][j] = 0;
		for (j = 0; j <= P; j++)
		{
			double sum = 0;
			for (int k = 0; k < j; k++)
				sum += (j - k)*Ai[n][k] * LPCEP[n][j - k];
			LPCEP[n][j] = -(Ai[n][j] + sum / (j + 1));
			h[n][j] = LPCEP[n][j];
		}
	}
	return n;																 //返回线性预测倒谱系数个数
}



/**********************************************************************
*        利用线性预测倒谱系数——LPCC系数计算线性预测倒谱——LPCC谱          *
*				LPCC谱为复数谱，所以也称为LPCC复倒谱                    *
*			线性预测倒谱保存在结构体 struct compx *h_Spectrum中         *
*						返回LPCC谱个数								  *
*                          作者：詹太安                                *
**********************************************************************/
int Wav_File_Work::LPCC(void)
{
	int i, n;
	h_Spectrum = (double*)malloc(sizeof(double)*Wav_File::Get_dataNumber() - N);//为Double型数据申请内存空间
	if (h_Spectrum == NULL) {												 //分配空间未成功
		throw invalid_argument("ERROR : Memory failure !");
	}
	for (i = 0, n = 0; i < Wav_File::Get_dataNumber() - N; i += Wav_File_Work::FrameShift, n++)
	{
		h_Spectrum[n] = 0;
		for (int k = 0; k <= P; k++)
		{
			h_Spectrum[n] += h[n][k] * cos(2 * PI*i*k / P);
		}
	}
	return n;																 //返回线性预测倒谱个数
}

void Wav_File_Work::ShowData(void)                                           //覆盖父类的展示数据函数
{
	//DFT();
	//FFT();
	//spectralEnergy(Wav_File::Get_dataNumber());
	//spectralCentroid(Wav_File::Get_dataNumber());							   //频谱质心
	//bandWidth(Wav_File::Get_dataNumber());								   //宽带
	//Subband_Energy_Ratio(Wav_File::Get_dataNumber());                        //子带能量比
	//Subband_Spectral_Flux(Wav_File::Get_dataNumber());					   //子带谱流量
	//Spectral_Rolloff_Point(Wav_File::Get_dataNumber());					   //频谱滚降点
	//LPC_Levinson_Durbin();												   //线性预测系数
	STFT();																	 //短时傅立叶变换
	/*
	LPC();																	 //线性预测谱
	LPCC_Coefficien();														 //线性预测倒谱系数
	LPCC();																	 //线性预测倒谱
	*/
}