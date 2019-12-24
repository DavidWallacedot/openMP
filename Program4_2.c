#include "omp.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#define ALIGNMENT 4096

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("usage: %s <num_threads> [stride]\n",argv[0]);
		return 1;
	}
	
	int num_threads = atoi(argv[1]);
	int stride = 1;
	
	if(argc >= 3)
		stride = atoi(argv[2]);

/* Donot need to pay attension to the following code. It is alligning the allocated array so that the start of the array is at the beining of achachline*/
/*************************************************************************/

	char* malloc_array = malloc(num_threads*stride+ALIGNMENT);
	uint64_t offset = ((uint64_t) malloc_array)%ALIGNMENT;
	char* aligned_array = ((void*)malloc_array)+ALIGNMENT-offset;
/*************************************************************************/
	
	for(int i = 0; i < num_threads; i+=stride)
		aligned_array[i]=0;
	
	double start_time = omp_get_wtime();
	
	
	#pragma omp parallel num_threads(num_threads)
	{
		int index = omp_get_thread_num()*stride;
		for(int i = 0; i < 3e8; i++)//300 million
			aligned_array[index]++;
			
	}
	
	
	double end_time = omp_get_wtime();
	
	printf("%f\n",end_time-start_time);
	
	return 0;
}
