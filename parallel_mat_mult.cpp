#include <stdlib.h>
#include <iostream>
#include <omp.h>
using namespace std;
int main(int argc, char * argv[]){
int n  =atoi( argv[1]);
static int numOfRows= n;
static int numOfCols = n;

double t1,t2 = 0;
int size = n*n;
int numberOfThreads = atoi(argv[2]);
omp_set_num_threads(numberOfThreads);
int i,nthreads;

double **matrixA = new double*[n];
for(int i = 0; i<n; ++i){
	matrixA[i] = new double[n];
} 

double ** matrixB = new double *[n];
for(int i = 0;i<n;++i){
	matrixB[i] = new double[n];
}

double ** matrixC = new double*[n];
for(int i = 0;i<n;++i ){
	matrixC[i] = new double[n];
}

srand(time(NULL));
for(int i =0 ; i<n; i++ ){
	for (int j = 0 ; j < n; j++){
		matrixA[i][j] = (double)rand();
		matrixB[i][j] = (double)rand();
	}
}
t1 = omp_get_wtime();
//This for loop is to compute the sequential processing time of matrix multiplication comment out if you do not want to run sequentially
for (int i = 0; i<n;i++){
	for(int j = 0; j<n;j++){
		matrixC [i][j]=0;
		for(int k = 0; k <n ; k ++){
			matrixC[i][j] += matrixA[i][k] * matrixB[k][j];
			
		}	
	}
}
t2 = omp_get_wtime();
cout<<"Time of execution for sequntial: "<<t2-t1<<endl<<endl<<endl;

for (int i = 0; i<n;i++){
	for(int j = 0; j<n;j++){
		matrixC [i][j]=0;
	}
}	
t1 = omp_get_wtime();
//this code executes the parallel algorithm that multiplies matrixes 
#pragma omp parallel
{	
	int i,id,nthrds;
	double x;
	id = omp_get_thread_num();
	nthrds = omp_get_num_threads();
	if(id==0) nthreads =nthrds;
	
	
	for ( int i = id; i < numOfRows; i+=nthrds )//traverse the rows
	  {
	    for (int j = 0; j < numOfCols; j++ )//traverse the columns
	    {
	      
	      for (int k = 0; k < n; k++ )
	      {
		matrixC[i][j] +=matrixA[i][k] * matrixB[k][j];
	      }
	    }
	  }
}
t2 = omp_get_wtime();

cout<<"Time of execution for parallel: "<<t2-t1<<endl<<endl<<endl;
for (int i = 0  ; i < n ; i++){
delete [] matrixA[i];
delete [] matrixB[i];
delete [] matrixC [i];

}
delete [] matrixA;
delete [] matrixB;
delete [] matrixC;

return 0;
}

