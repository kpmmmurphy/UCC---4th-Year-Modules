#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void compare_and_exchange(int n, double * a, int rank, MPI_Status status);

//Function Declerations
int MPI_Odd_even_sort(int n, double * array, int root, MPI_Comm comm);
int MPI_Exhange(int n, double * array, int rank1, int rank2, MPI_Comm comm);
int MPI_Is_sorted(int n, double * array, int root, MPI_Comm comm);

double * genRandomArray(int size);
double * merge_array(int n, double * a, int m, double * b);
void merge(double a[], int n, double  b[], int m, double sorted[]);
void merge_sort(int n, double * a);
void swap (double * a, double * b);

void print_array(double * array, int size);

int rank, size;
MPI_Status status;
double *a, *b, *c, *localArray;
double m = 10.0;
int n = 1000000, q, l, i, j, isSorted = -1, x, *nr, count = 0;

double totalTime, exchangeTime;

int main (int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
       
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    b          = (double *)calloc(n,      sizeof(double));
    c          = (double *)calloc(n,      sizeof(double));
    localArray = (double *)calloc(n/size, sizeof(double));
    
    if(rank == 0)
    {
        a = genRandomArray(n);
	totalTime = MPI_Wtime();
    }

    MPI_Odd_even_sort(n, a, 0, MPI_COMM_WORLD);
    
    if(rank == 0)
    {
      totalTime = MPI_Wtime() - totalTime;
      
      
      for(x = 0; x < n; x++)
      {
	//printf("Output : %f\n", a[x]);
      }
      
      printf("\nExecution Time: %f\n", totalTime);
    }
    
    MPI_Finalize();
}

// ---------------------------------------------
// these functions deal with sorting and merging
//----------------------------------------------

double * genRandomArray(int size)
{
    a = (double *) calloc(n, sizeof(double));
    
    //initialise the array with random values
    srand( ((unsigned)time(NULL)+rank) );

    for( i = 0; i < size; i++ )
    {
        a[i]=((double)rand()/RAND_MAX)*m;
        //printf( "Initial: %f\n", a[i] );
    } 

    return a;
}

int MPI_Exchange(int n, double * array, int rank1, int rank2, MPI_Comm comm)
{
    int tag1 = 0, tag2 = 1, i;

    if(rank==rank1)
    {
	//Send Unsorted Array
	MPI_Send(array, n, MPI_DOUBLE, rank2, tag1, comm);

	//Recieve
	MPI_Recv(b, n, MPI_DOUBLE, rank2, tag2, comm, &status);

	//Merge
	c = merge_array(n, array, n, b);

	for(i=0;i<n;i++){
	     array[i] = c[i];
	}
    }

    if(rank==rank2)
    {	        
        //Recieve 
	MPI_Recv(b, n, MPI_DOUBLE, rank1, tag1, comm, &status);
      
	//Send Unsorted Array
	MPI_Send(array, n, MPI_DOUBLE, rank1, tag2, comm);

	//Merge
	c = merge_array(n, array, n, b);

	for(i=0;i<n;i++){
	     array[i] = c[i+n];
        }
    }

    return MPI_SUCCESS; 
}

int MPI_Odd_even_sort(int n, double * array, int root, MPI_Comm comm)
{
	//Scatter
        MPI_Scatter(array, n/size, MPI_DOUBLE, localArray, n/size, MPI_DOUBLE, 0, comm);
	
        //Sort localArray
        merge_sort(n/size, localArray);

	//Time the Exchange
	exchangeTime = MPI_Wtime();
	
	//Repeart for step=0,1,2..., size-1
	for(i=0; i <= size; i++)
	{
	    if(isSorted == -1)
            {	
	        if(((rank + i) % 2) == 0)
	        {
		    if(rank < size - 1)
		    {
		        MPI_Exchange(n/size, localArray, rank, rank+1, comm);
		    }
	        }
	        else
	        {
	            if(rank > 0)
		    {
		        MPI_Exchange(n/size, localArray, rank-1, rank, comm);
		    }
	        }
            }
	    else
	    {
		break;
	    }
	    
	    MPI_Is_sorted(n/size, localArray, 0, comm);
	    MPI_Barrier(comm);
	}
	
	//Finish Timing Exchange
	exchangeTime = MPI_Wtime() - exchangeTime;
	printf("Exchange Time: %f\n", exchangeTime);

	//Gather first elements of array to root
	MPI_Gather(localArray, n/size, MPI_DOUBLE, array, n/size , MPI_DOUBLE, 0, comm);

	return MPI_SUCCESS;
}

int MPI_Is_sorted(int num, double * array, int root,  MPI_Comm comm)
{
    int _isSorted = -1;
    
    double *first, *last;
    first = (double *)calloc(num, sizeof(double));
    last  = (double *)calloc(num, sizeof(double));

    MPI_Gather(&array[0], 1, MPI_DOUBLE, &first[0], 1, MPI_DOUBLE, 0, comm);
    MPI_Gather(&array[num-1], 1, MPI_DOUBLE, &last[0], 1, MPI_DOUBLE, 0, comm);
    
    if(rank == root)
    {
	for(i = 1; i < num; i++)
	{
	    //if(last[i - 1] < first[i])
	    //{
	    //    isSorted = 1;
	    //}
	    if(first[i + 1] < last[i])
	    {
		isSorted = 1;
	    }
	}
    }
    MPI_Bcast(&isSorted, 1 , MPI_INT, 0, comm);
    return MPI_SUCCESS;
}

double * merge_array(int n, double * a, int m, double * b)
{
   int i,j,k;
   double * f = (double *) calloc(n+m, sizeof(double));

   for(i=j=k=0;(i<n)&&(j<m);)
      if(a[i]<=b[j])f[k++]=a[i++];
      else f[k++]=b[j++];
      if(i==n)for(;j<m;)f[k++]=b[j++];
      else for(;i<n;)f[k++]=a[i++];
   
   return f;
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

void print_array(double *array, int size)
{
    int i;
    for(i = 0 ; i < size ; i++) {
        printf("%f \n", array[i]);
    }
}
