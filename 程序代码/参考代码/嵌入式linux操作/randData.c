#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int main()
{
	float *graph_data;
	srand((unsigned)time(NULL));
    graph_data = new float[450];
    for (int i = 0; i < 450; ++i){
        graph_data[i] = rand() % 100;
        if(rand()%2){
            graph_data[i] *= -1;
        }
        graph_data[i] /= 100;
        printf("%lf \t", graph_data[i]);
    }
	return 0;
}