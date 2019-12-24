//Author David Wallace
// purpose: program takes the input from CL and lets that input = n and finds i to n summation. This program compares the sequential time to 
// parallel time
// Date: 9/7/19
#include <iostream>
#include <cstdlib>
#include <omp.h> 
using namespace std;
int main(int argc, char** argv){
	
	//sequential summation
	
	//local variables used to store data
	double t1,t2 = 0;
	unsigned long long int n = atoi(argv[1]);
	int numOfThreads = atoi(argv[2]);
	unsigned long long int sum = 0; 
	
	//sequential code to sum a list of numbers
	t1 = omp_get_wtime();
	for (unsigned long long int i = 0; i <= n;i++){
		sum +=i; 
	}
	
	t2 = omp_get_wtime();
	cout<< "Sum for sequential : "<<sum <<endl;
	cout<<"Time of execution for sequential: "<<t2-t1<<endl<<endl<<endl;
	

	//parallel summation

	//set variables
	sum=0;
	t1 = omp_get_wtime();
	//parallel code to get the sum of numbers
	#pragma omp parallel for reduction(+:sum) num_threads(numOfThreads)
		for(unsigned long long int i = 0; i <=n; i++ )
		{	
			sum +=i;
		}
	t2 = omp_get_wtime();
	cout<<"Sum for parallel: "<<sum<<endl; 

	cout<<"Time of execution for parallel: "<<t2-t1<<endl;
	return 0;
}

