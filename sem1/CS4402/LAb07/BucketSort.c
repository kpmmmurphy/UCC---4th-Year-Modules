#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

double * merge_array(int n, double * a, int m, double * b);
void merge_sort(int n, double * a);
void swap (double * a, double * b);

int main (int argc, char *argv[])
{

	int rank, size;

	int n = 16, q, l, i, j, k, x, *nr, count = 0;
	double m = 10.0;
	double *a, *bucket;
	int * bucketCount, * displs;

	MPI_Status status;

	MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	a = (double *) calloc(n,sizeof(double));
	bucket = (double *) calloc(n,sizeof(double));
	bucketCount = (int *) calloc(size, sizeof(int));
	displs = (int *) calloc(size, sizeof(int));

	if( rank == 0 )
	{

	   //initialise the array with random values, then scatter to all processors
	   srand( ((unsigned)time(NULL)+rank) );

	   for( i = 0; i < n; i++ )
	   {
	      a[i]=((double)rand()/RAND_MAX)*m;
	      printf( "Initial: %f\n", a[i] );
	   }

	}

	// Braodcast the array to the processor
	MPI_Bcast(&a[0], n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	// P rank collects the elements of its bucket between m*rank/size and m*(rank+1)/size 
	for(i=0;i<n;i++){
		if(a[i]>=m*rank/size && a[i] < m*(rank+1)/size){
			bucket[count++] = a[i];
		}
	}
	// P rank sorts its bucket
	merge_sort(count, bucket);
	// the numbers of elements in buckets are gathered to the root
	MPI_Gather(&count, 1, MPI_INT, &bucketCount[0], 1, MPI_INT, 0, MPI_COMM_WORLD);
	if(rank==0){
		displs[0] = 0;
		for(i=0; i<size ; i++){
			displs[i] = displs[i-1] + bucketCount[i-1];
		}
	}

	// the buckets are v-gathered to the root
	MPI_Gatherv(&bucket[0], count, MPI_DOUBLE, &a[0], bucketCount, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if( rank == 0 )
	{
	   for( i = 0; i < n; i++ )
	   {
	      printf( "Output : %f\n", a[i] );
	   }
	}
	MPI_Finalize();

}



// ------------------------------------------------------------
//
// these functions deal with sorting and merging
//

double * merge_array(int n, double * a, int m, double * b){

   int i,j,k;
   double * c = (double *) calloc(n+m, sizeof(double));

   for(i=j=k=0;(i<n)&&(j<m);)
      if(a[i]<=b[j])c[k++]=a[i++];
      else c[k++]=b[j++];
      if(i==n)for(;j<m;)c[k++]=b[j++];
      else for(;i<n;)c[k++]=a[i++];
   return c;
}

void merge_sort(int n, double * a){

   double * c;
   int i;

   if (n<=1) return;

   if(n==2) {

      if(a[0]>a[1])swap(&a[0],&a[1]);
      return;
   }



   merge_sort(n/2,a);merge_sort(n-n/2,a+n/2);

   c=merge_array(n/2,a,n-n/2,a+n/2);

   for(i=0;i<n;i++)a[i]=c[i];

}

void swap (double * a, double * b){

   double temp;

   temp=*a;*a=*b;*b=temp;

}
