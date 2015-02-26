/***************************************************************

 MPI program to compute an array operation

****************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int MPI_Array_sum(int n, int *array, int * finalSum, int root, MPI_Comm comm );

int main(int argc, char * argv []) {


	int * scattered_array, * array;
	int n = 10000000; int n1;
	int size, rank;
	int i;

	// declare and initialise the variables for sum, prod, max and min
	int sum = 0, final_sum = 0;

	// Init + rank + size
	MPI_Init (&argc,&argv);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);

	scattered_array=(int *)calloc(n/size, sizeof(int));

	// 	populate the array a if processor 0
	// 	serial code
	if (rank == 0) {

	   // Processor 0 is the only one to know array
	   array = (int *)calloc(n, sizeof(int));
	   for(i=0;i<n;i++)array[i]=1; // you can initialise with a random number


	  }

	// scatter the array onto processors
	// local array is scattered_array
	MPI_Scatter(array, n/size, MPI_INT, scattered_array, n/size, MPI_INT, 0, MPI_COMM_WORLD);


	// calculate sum, prod, max and min of scattered_array
	for(i=0;i<n/size;i++)
	{
		sum+=scattered_array[i];
	}

	// reduce sum, prod, max, min to final_sum, final_prod, final_max and final_min
	//MPI_Reduce(&sum,&final_sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	MPI_Array_sum(n, array, &final_sum, 0, MPI_COMM_WORLD);

	if(rank==0){
	   // Write final_sum, final_prod, final_max and final_min
	   printf("the final sum is %d", final_sum);
	}

	MPI_Finalize ();

}

int MPI_Array_sum(int n, int *array, int * finalSum, int root, MPI_Comm comm )
{
	int rank, size, error, i, localSum = 0;
	int *localArray;

	//Aquire the comm size and rank
	MPI_Comm_size(comm, &size);
	MPI_Comm_rank(comm, &rank);

	localArray = (int *) calloc(n/size, sizeof(int));

	//Scatter array to local Array
	error = MPI_Scatter(array, n/size, MPI_INT, localArray, n/size, MPI_INT, root, comm);
	if(error != MPI_SUCCESS) return error;

	//Compute localArray
	for(i=0; i < n/size ; i++)
	{
		localSum += localArray[i];
	}
	//reduce localSum
	error = MPI_Reduce(&localSum, finalSum, 1, MPI_INT, MPI_SUM, root, comm);
	if(error != MPI_SUCCESS) return error;
}
