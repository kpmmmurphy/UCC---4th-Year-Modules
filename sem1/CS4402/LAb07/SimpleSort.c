#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main (int argc, char *argv[])
{

	int rank, size;

	int n = 16000000, q, l, i, j, k, x, *nr;
	double m = 10.0;
	double *a, *b, time, totalTime;
	int *ranking, *finalRanking;

	MPI_Status status;

	MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	a = (double *) calloc(n,sizeof(double));
	b = (double *) calloc(n,sizeof(double));

	//time = (double *) calloc(n, sizeof(double));
	//totalTime = (double *) calloc(n, sizeof(double))
	
	ranking = (int *) calloc(n/size, sizeof(int));
	finalRanking = (int *) calloc(n, sizeof(int));
	if( rank == 0 )
	{

	   //initialise the array with random values, then scatter to all processors
	   srand( ((unsigned)time(NULL)+rank) );

	   for( i = 0; i < n; i++ )
	   {
	      a[i]=((double)rand()/RAND_MAX)*m;
	      //printf( "Initial: %f\n", a[i] );
	   }

	}
	time = MPI_Wtime();
	// Braodcast the array to the processor
	MPI_Bcast(&a[0], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	// P rank generates an array ranking with ranking[i] is the rank of a[i+rank*n/size] in the array
	for(i=0;i<n/size;i++){
	    ranking[i] = 0;

	    for(j=0;j<n;j++){

	        if(a[i+rank*n/size]>a[j]){
		    ranking[i]++;
		}

	    }
	}
		
	// Gather the array ranking to finalRanking
	MPI_Gather(&ranking[0], n/size, MPI_INT, &finalRanking[0], n/size, MPI_INT, 0, MPI_COMM_WORLD);

	//if processor 0 then restore the order in the array b	
	if( rank == 0 )
	{
	   for( i = 0; i < n; i++ )
	   {
	      b[finalRanking[i]] = a[i];
	   }
	}	

	time = MPI_Wtime() - time;

	MPI_Reduce(&time, &totalTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if(rank == 0){
	    printf("Sorting %d numbers with %d procs take %lf", n, size, totalTime);
	    for(i=0;i<n;i++){
		//printf("Output: %f\n", b[i]);
	    }
	}

	MPI_Finalize();

}


