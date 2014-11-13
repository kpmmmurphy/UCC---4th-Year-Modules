#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void compare_and_exchange(int n, double * a, int rank, MPI_Status status);

//Function Declerations
int MPI_Odd_even_sort(int n, double * array, int root, MPI_Comm comm);
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
    if(rank == 0)
    {
        a = genRandomArray(n);
    }	
    MPI_Odd_even_sort(n, a, 0, MPI_COMM_WORLD);
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

    if(rank==rank1)
    {
	//Send Unsorted Array
	MPI_Send(array, n, MPI_DOUBLE, rank2, tag1, comm);

	//Recieve
	MPI_Recv(b, n, MPI_DOUBLE, rank2, tag2, comm, &status);
	printf("Recieved from rank2");
	//Merge
	c = merge_array(n, array, n, b);

	for(i=0;i<n;i++){
	     array[i] = c[i];
	}
	print_array(array, n);
    }

    if(rank==rank2)
    {	        
	//Send Unsorted Array
	MPI_Send(array, n, MPI_DOUBLE, rank1, tag2, comm);

	//Recieve 
	MPI_Recv(b, n, MPI_DOUBLE, rank1, tag1, comm, &status);
	printf("Revieved from rank1");
	//Merge
	c = merge_array(n, array, n, b);

	for(i=0;i<n;i++){
	     array[i] = c[i+n];
        }
	print_array(array, n);
    }

    return MPI_SUCCESS; //MPI_SUCCESS;
}

int MPI_Odd_even_sort(int n, double * array, int root, MPI_Comm comm)
{
	double * localArray = (double *)calloc(n/size, sizeof(double));

	//Scatter
        MPI_Scatter(&array[0], n/size, MPI_DOUBLE, localArray, n/size, MPI_DOUBLE, 0, comm);
	
        //Sort localArray
        merge_sort(n/size, localArray);

	printf("Merged Local Array Rank: %d\n", rank);
	print_array(localArray, n/size);

	//Repeart for step=0,1,2..., size-1
	for(i=0; i < size; i++)
	{
	    printf("Loop: %d, Rank: %d \n", i, rank);

	    if(((rank) % 2) == 0)
	    {
		printf("Rank %d, Even\n", rank);
		if(rank < size - 1)
		{
		    MPI_Exchange(n/size, localArray, rank, rank+1, comm);
		}
	    }
	    else
	    {
		    printf("Rank %d, Odd\n", rank);
		    if(rank > 0)
		    {
		        MPI_Exchange(n/size, localArray, rank-1, rank, comm);
		    }
	    }
	
	    //MPI_Barrier(comm);
	}
	printf("hERE");

	if(rank == root)
	{
	    //Gather
			
            //MPI_Gather(&localArray[0], n/size, MPI_DOUBLE, &array[0], n/size, MPI_DOUBLE, 0, comm);

	    //Gather first elements of array to root
	    MPI_Gather(&localArray[0], n/size, MPI_DOUBLE, &array[0], n/size, MPI_DOUBLE, 0, comm);
	    printf("Gathered Elements");
	    print_array(array, 10);
	    for(x = 0; x < n; x++)
	    {
		printf("Output : %f\n", array[x]);
	    }
	}

	return MPI_SUCCESS;
}

int MPI_Is_sorted(int n, double * array, int rank1, int rank2, MPI_Comm comm)
{
    
    return MPI_SUCCESS;
}

double * merge_array(int n, double * a, int m, double * b){

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
    //int length = (int) sizeof(array) / sizeof(array[0]);
    for(i = 0 ; i < size ; i++) {
        printf("%f \n", array[i]);
    }
}
