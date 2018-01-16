#include <iostream>
using namespace std;

const int N = 8;                                                             //窗长
int array[N] = { 0, 1, 2, 3, 4, 5, 6, 7 };                                   //测试数据

int main(void)
{

	int endNum;
	int temp;
	endNum = 0;

	for (int i = 0; i < N - 1; i++)                                          //因为第一个数永远是0不变，所以查找次数为N-1
	{
		if (i < endNum)                                                      //如果顺序数小于逆序数则交换，这是防止重复交换的一种手段，改为大于号也行
		{
			temp = array[i];                                                 //交换过程
			array[i] = array[endNum];
			array[endNum] = temp;
		}

		int k = N >> 1;                                                      //100 代表窗长的一半，用来判断最高位是否为1

		while (k <= endNum)                                                  //如果逆序数的最高位为1
		{
			endNum = endNum - k;                                             //则当前位置0
			k >>= 1;                                                         //基标/2
		}

		endNum = endNum + k;                                                 //当前位为0，则逆转为1
	}

	for (int i = 0; i < N; i++)
		printf("%d ", array[i]);
	printf("\n");
	return 0;
}