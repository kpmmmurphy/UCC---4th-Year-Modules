#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void compare_and_exchange(int n, double * a, int rank, MPI_Status status);

//Function Declerations
int MPI_Sort(int n, double * array, int root, MPI_Comm comm);
int MPI_Exhange(int n, double * array, int rank1, int rank2, MPI_Comm comm);
int MPI_Is_sorted(int n, double * array, int rank1, int rank2, MPI_Comm comm);

double * genRandomArray(int size);
double * merge_array(int n, double * a, int m, double * b);
void merge(double a[], int n, double  b[], int m, double sorted[]);
void merge_sort(int n, double * a);
void swap (double * a, double * b);

void print_array(double * array, int size);

int rank, size;
MPI_Status status;
double *a, *b, *c;
double m = 10.0;
int n = 10, q, l, i, j, k, x, *nr, count = 0;

int main (int argc, char *argv[])
{

    MPI_Init(&argc, &argv);
       
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    a = genRandomArray(n);

    MPI_Exchange(n, a, 0, 1, MPI_COMM_WORLD);

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
        printf( "Initial: %f\n", a[i] );
    } 

    return a;
}

int MPI_Exchange(int n, double * array, int rank1, int rank2, MPI_Comm comm)
{
    int tag1 = 0, tag2 = 1, i;



    b = (double *)calloc(n, sizeof(double));
    //c = (double *)calloc(n * 2, sizeof(double));

    printf("Calling MPI_Exchange...From Rank %d\n\n", rank);

    if(rank==rank1)
    {
	//Send Unsorted Array
	MPI_Send(array, n, MPI_DOUBLE,i rank2, tag1, comm);

	//Recieve
	MPI_Recv(b, n, MPI_DOUBLE, rank2, tag2, comm, &status);
	
	printf("Rank %d Recieved Array B: \n ", rank);
	print_array(b, n);

	//Merge
	c = merge_array(n, array, n, b);
 	
	printf("Rank %d Merged Array\n", rank);
	for(i=0;i<n;i++){
	     array[i] = c[i];
	}
	print_array(array, n);
    }

    if(rank==rank2)
    {	
	//Recieve
	MPI_Recv(b, n, MPI_DOUBLE, rank1, tag1, comm, &status);
        
	//printf("Rank %d Recieved Array B: \n", rank);
	//print_array(b, n);

	//Send Unsorted Array
	MPI_Send(array, n, MPI_DOUBLE, rank1, tag2, comm);

	//Merge
	c = merge_array(n, array, n, b);

	//printf("Rank %d Merged Array\n", rank);

	for(i=0;i<n;i++){
	     array[i] = c[i+n];
        }
	//print_array(array, n);
    }

    return MPI_SUCCESS; //MPI_SUCCESS;
}

int MPI_Sort(int n, double * array, int root, MPI_Comm comm);
{
	double * localArray = (double *)calloc(n/size, sizeof(double));
	//Scatter
        MPI_Scatter(array, n/size, MPI_DOUBLE, localArray, n/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // sort scattered_array
        merger_sort(n/size, localArray)

	//Repeart for step=0,1,2..., size-1
	for(i=0; i < size; i++)
	{
	    if((i+rank) % 2 == 0)
	    {
		if(rank < size-1)
		{
		    MPI_Exchange(n/size, localArray, rank, rank+1, MPI_COMM_WORLD);
		}
		else
		{
		    if(rank > 0)
		    {
		        MPI_Exchange(n/size, localArray, rank-1, rank, MPI_COMM_WORLD);
		    }
		}
	    }
	    MPI_Barrier(MPI_COMM_WORLD);
	}

        //Gather
        MPI_Gather(localArray, n/size, MPI_Double, array, n/size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if(rank == 0)
	{
	    for(x = 0; x < n; x++)
	    {
		printf("Output : %f\n", array[x]);
	    }
	}
}


double * merge_array(int n, double * a, int m, double * b){
   int i,j,k;
   double * f = (double *) calloc(n+m, sizeof(double));

   for(i=j=k=0;(i<n)&&(j<m);){
      if(a[i]<=b[j])f[k++]=a[i++];
      else f[k++]=b[j++];
      if(i==n)for(;j<m;)f[k++]=b[j++];
      else for(;i<n;)f[k++]=a[i++];
   }
   printf("MERGE_ARRAY\n");
   print_array(f, n + m);
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
    //int length = (int) sizeof(array) / sizeof(array[0]);
    for(i = 0 ; i < size ; i++) {
        printf("%f \n", array[i]);
    }
}
