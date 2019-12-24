#include "stdio.h"
#include <stdlib.h>
#include "omp.h"
#include "malloc.h"
#include "stdint.h"
#include "inttypes.h"

const uint64_t BYTES_PER_LOOP = 4*4*8; //4 avx registers in loop and 4 doubles per avx register
const uint64_t ALIGNMENT = 4096;

void asm_load(double* array, uint64_t size, uint64_t iters)
{
	asm volatile (
	"xor rax, rax"										"\n\t"
	".IterLoop%=:"										"\n\t"//loop 'iters' times
		"inc rax"										"\n\t"
		"xor rbx, rbx"									"\n\t"
		".RowsLoop%=:"									"\n\t"//loop over the array ('size' bytes) and load each value
			"add rbx, 128"								"\n\t"
			"vmovapd ymm0, YMMWORD PTR [%[arr]+rbx]"	"\n\t"//load 4 doubles into registers (32 bytes)
			"vmovapd ymm1, YMMWORD PTR [%[arr]+rbx+32]"	"\n\t"//" "
			"vmovapd ymm2, YMMWORD PTR [%[arr]+rbx+64]"	"\n\t"//" "
			"vmovapd ymm3, YMMWORD PTR [%[arr]+rbx+96]"	"\n\t"//" "
			"cmp rbx, %[len]"							"\n\t"
		"jne .RowsLoop%="								"\n\t"
		"cmp rax,%[iters]"								"\n\t"
	"jne .IterLoop%="									"\n\t"
	:
	:[arr] "r"(array), [len] "r"(size), [iters] "r"(iters)
	:"rax", "rbx", "memory", "cc"
	);
}

void load(double* aligned_array, uint64_t size, uint64_t iters)
/*This function read the array <aligned_array> of with <size> elements <iters> times repeatedly. You donot have to expalin this function*/
{	
	double start_time = omp_get_wtime();

	asm_load(aligned_array, size, iters);

	double end_time = omp_get_wtime();
	
	uint64_t bytes_read = size * iters;
	
	double gb_per_s = bytes_read / (end_time-start_time) / (1024*1024*1024);

	printf("%" PRIu64 " %f\n",size,gb_per_s);
	
}

int main(int argc, char* argv[])
{
	uint64_t start = BYTES_PER_LOOP; //bytes
	uint64_t end = 128*1024*1024; //bytes
	uint64_t steps = 16;  //steps per binary order of magnitude
	uint64_t min_divisiblity = BYTES_PER_LOOP;
	uint64_t total_bytes_to_read = 1LL<<35;

/*The following code aligns the pointer to a multiple of ALIGNMENT.  You do not have to explain this*/
/*****************************************************************************************************/	
	double* malloc_array = malloc(end+ALIGNMENT);
	uint64_t offset = ((uint64_t) malloc_array)%ALIGNMENT;
	double* aligned_array = ((void*)malloc_array)+ALIGNMENT-offset;
/****************************************************************************************************/		
	
	for(uint64_t i = 0; i < end/sizeof(double); i++)
		aligned_array[i]=i;
	
	//printf("Array size (bytes)     Read rate (GB/s)\n");

	for(uint64_t i = start; i < end; i*=2)
	{
		printf("%li \n",i);
		
		//printf("iter on outer for loop\n");
		for(uint64_t j = 0; j < steps; j++)
		{	//printf("inside inner for loop \n");
			
			uint64_t size = i*(j+steps)/steps;
						
			if(size % min_divisiblity == 0){
				printf("size %ld",size);				
				load(aligned_array, size,total_bytes_to_read/size);
				}		
		}
	}
	
	free(malloc_array);
}
