#include<vector>
#include<iostream>
#include<stdlib.h>
#include<errno.h>
#include<omp.h>
#include <fstream>
#include<string>
using namespace std;

int main(int argc, char *argv[]){

//instantiate arrays 
int rows = atoi(argv[2])+2;
int cols = atoi(argv[3])+2;
double **newArrayPtr = new double*[rows];

for(int i = 0; i < rows;i++){
	newArrayPtr[i] = new double[cols];
}

double **oldArrayPtr = new double*[rows];
for(int i = 0; i < rows;i++){
	oldArrayPtr[i] = new double[cols];
}

//initialize values to have default values of border temperature
for(int i =0 ; i < rows;i++){
	for(int j = 0; j < cols;j++){
		newArrayPtr [i][j] = atof(argv[5]);
		oldArrayPtr [i][j] = atof(argv[5]);
	}	
	
}
vector <double> v; 
double value;
fstream fixed_temp_file ;
fixed_temp_file.open(argv[7]);
if(fixed_temp_file.is_open()){
	while(fixed_temp_file>>value)
	{	
		v.push_back(value);
	}
}
int x = 0; 
int y = 0; 
//initialize array to have constant heat sources 
for (int i = 1; i < v.size(); i+=3){
	x = v.at(i)+1;
	y  = v.at(i+1)+1;
	oldArrayPtr[x][y]= v.at(i+2);

}


double **tempArrayPtr;
double t1 = omp_get_wtime();
for(int z = 0 ; z < atoi(argv[6]); z ++ ){
	tempArrayPtr = oldArrayPtr;
	double k = atof(argv[4]);

	#pragma omp  parallel num_threads(atoi(argv[1]))
	{
		int start, end, id; 
		id = omp_get_thread_num();
		start = id * (rows)/omp_get_num_threads() + 1;
		end = start + (rows)/omp_get_num_threads()+1;
		if(id == omp_get_num_threads()-1){end = rows-1; }
	double newValue; 
	for (int i = start ; i< end; i++){
		for(int j = 1 ; j < cols-1; j ++){
			newValue  =  (k * (*(*(oldArrayPtr+i-1)+j-1) + (*(*(oldArrayPtr+i-1)+j)) + (*(*(oldArrayPtr+i-1)+j+1))+ (*(*(oldArrayPtr+i)+j-1))  + (*(*(oldArrayPtr+i)+j+1)) + (*(*(oldArrayPtr+i+1)+j-1)) + (*(*(oldArrayPtr+i+1)+j))+ (*(*(oldArrayPtr+i+1)+j+1)))/8) + (1-k) * (*(*(oldArrayPtr+i)+j));
	(*(*(newArrayPtr+i)+j)) = newValue;

			}
		}
	}//pragma


	oldArrayPtr = newArrayPtr; 
	for (int i = 1; i < v.size(); i+=3){
		x = v.at(i)+1;
		y  = v.at(i+1)+1;
		oldArrayPtr[x][y]= v.at(i+2);

	}
	newArrayPtr = tempArrayPtr; 
}
double t2 = omp_get_wtime();

//print out 
for(int i =0 ; i < rows;i++){
	for(int j = 0; j < cols;j++){
		cout<< oldArrayPtr[i][j]<<" ";
	}	
 		cout<<endl;
}

for (int i = 0 ; i < rows ; ++i)
{
delete [] newArrayPtr[i];
delete [] oldArrayPtr[i];

}

delete [] newArrayPtr; 
delete [] oldArrayPtr;
cout<<t2-t1<<endl;
return 0;
}


