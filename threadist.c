#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int p[20];
int *a = p, count = 0;
int* consumption(int size,int consumer) {
	
	int i = size/consumer, j = size;
	do {
		if (consumer == 1) {
			*a = j;
			++count;
		}
		else {
			*a = i;
			++count;
		}
		a++;
		j = size - i;
		consumer--;
	}while(consumer > 0);
	
	a = 0;
	return a;
}

void main(int argc, char* argv[])
{

	//	store buffer size, number of producers and number of consumers
	int buffer_size = atoi(argv[1]); 
	int producercount = atoi(argv[2]); 
	int consumercount = atoi(argv[3]);	
	
	
	int *t = consumption(buffer_size, consumercount);
	
	printf("\n number of values per thread: ");
	for (int k = 0;k<count; t++, k++) {
		printf("%d ",*t);
	}
}