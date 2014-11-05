#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void compare_and_exchange(int n, double * a, int rank, MPI_Status status);

//Function Declerations
int MPI_Sort(int n, double * array, int root, MPI_Comm comm);
int MPI_Exhange(int n, double * array, int rank1, int rank2, MPI_Comm comm);
int MPI_Is_sorted(int n, double * array, int rank1, int rank2, MPI_Comm comm);

double * merge_array(int n, double * a, int m, double * b);
void merge_sort(int n, double * a);
void swap (double * a, double * b);
void print_array(double * array);

int rank, size;
MPI_Status status;
double *c;

int main (int argc, char *argv[])
{

	int n = 16, q, l, i, j, k, x, *nr, count = 0;
	double unsorted_array[10] = {10.1,11.2,3.5,4.7,8.7,1.7,1.6,2.6,3.5,99.5};

	MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("About to call MPI_Exchange...\n");
	c = (double *)calloc(n, sizeof(double));

	MPI_Exchange(10, unsorted_array, 0, 1, MPI_COMM_WORLD);

	MPI_Finalize();
}

// ---------------------------------------------
// these functions deal with sorting and merging
//----------------------------------------------

int MPI_Exchange(int n, double * array, int rank1, int rank2, MPI_Comm comm)
{
    double * b = (double *)calloc(n, sizeof(double));
    int tag1 = 0, tag2 = 1, i;

    printf("Calling MPI_Exchange...From Rank %d\n", rank);

    if(rank==rank1)
    {
	//Send Unsorted Array
	MPI_Send(&array[0], n, MPI_DOUBLE, rank2, tag1, comm);

	//Recieve
	MPI_Recv(b, n, MPI_DOUBLE, rank2, tag2, comm, &status);
	printf("Rank %d Recieved Array: \n ", rank);
	print_array(b);

	//Merge Arrays
	c = merge_array(n, array, n, b);

	for(i=0;i<n++;i++) array[i] = c[i];
	printf("Rank %d, Sorted Array\n", rank);
	print_array(c);
    }

    if(rank==rank2)
    {
	//Send Unsorted Array
	MPI_Send(&array[0], n, MPI_DOUBLE, rank1, tag2, comm);

	//Recieve
	MPI_Recv(b, n, MPI_DOUBLE, rank1, tag1, comm, &status);
        printf("Rank %d Recieved Array: \n", rank);
	print_array(b);

	//Merge
	c = merge_array(n, array, n, b);

	for(i=0;i<n;i++) array[i] = c[i+1];
	printf("Rank %d Sorted Array\n", rank);
	print_array(c);
    }
}

double * merge_array(int n, double * a, int m, double * b){

   int i,j,k;
   double * c = (double *) calloc(n+m, sizeof(double));

   for(i=j=k=0;(i<n)&&(j<m);){
      if(a[i]<=b[j])c[k++]=a[i++];
      else c[k++]=b[j++];
      if(i==n)for(;j<m;)c[k++]=b[j++];
      else for(;i<n;)c[k++]=a[i++];
   }
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

void print_array(double *array)
{
    int i;
    for(i = 0; i < sizeof(array); i++) {
        printf(" %f \n", array[i]);
    }
}
