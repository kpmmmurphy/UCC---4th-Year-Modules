#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void compare_and_exchange(int n, double * a, int rank, MPI_Status status);

//Function Declerations
int MPI_Sort(int n, double * array, int root, MPI_Comm comm);
int MPI_Exhange(int n, double * array, int rank1, int rank2, MPI_Comm comm);
int MPI_Is_sorted(int n, double * array, int rank1, int rank2, MPI_Comm comm);

void merge(int n, double * a, int m, double * b, double * sorted);
void merge_sort(int n, double * a);
void swap (double * a, double * b);
void print_array(double * array);

int rank, size;
MPI_Status status;
double *b, *c;

int main (int argc, char *argv[])
{

	int n = 10, q, l, i, j, k, x, *nr, count = 0;
	double unsorted_array[] = {10.1,11.2,3.5,4.7,8.7,1.7,1.6,2.6,3.5,99.5};

	MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Exchange(n, unsorted_array, 0, 1, MPI_COMM_WORLD);

	MPI_Finalize();
}

// ---------------------------------------------
// these functions deal with sorting and merging
//----------------------------------------------

int MPI_Exchange(int n, double * array, int rank1, int rank2, MPI_Comm comm)
{
    int tag1 = 0, tag2 = 1, i;

    b = (double *)calloc(n, sizeof(double));
    c = (double *)calloc(n, sizeof(double));

    printf("Unsorted %d\n", n);
    print_array(array);

    printf("Calling MPI_Exchange...From Rank %d\n\n", rank);

    if(rank==rank1)
    {
	//Send Unsorted Array
	MPI_Send(&array[0], n, MPI_DOUBLE, rank2, tag1, comm);

	//Recieve
	MPI_Recv(&b[0], n, MPI_DOUBLE, rank2, tag2, comm, &status);
	printf("Rank %d Recieved Array: \n ", rank);
	print_array(b);

	//Merge
	merge(n, array, n, b, c);

	printf("Rank %d Merged Array\n", rank);

	for(i=0;i<n;i++) array[i] = c[i];
	print_array(array);
    }

    if(rank==rank2)
    {	
	//Recieve
	MPI_Recv(&b[0], n, MPI_DOUBLE, rank1, tag1, comm, &status);
        printf("Rank %d Recieved Array: \n", rank);
	print_array(b);

	//Send Unsorted Array
	MPI_Send(&array[0], n, MPI_DOUBLE, rank1, tag2, comm);

	//Merge
	merge(n, array, n, b, c);

	printf("Rank %d Merged Array\n", rank);

	for(i=0;i<n;i++) array[i] = c[i+n];
	print_array(array);
    }
}

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



void merge(int a[], int m, int b[], int n, int sorted[]) {
  int i, j, k;
 
  j = k = 0;
 
  for (i = 0; i < m + n;) {
    if (j < m && k < n) {
      if (a[j] < b[k]) {
        sorted[i] = a[j];
        j++;
      }
      else {
        sorted[i] = b[k];
        k++;
      }
      i++;
    }
    else if (j == m) {
      for (; i < m + n;) {
        sorted[i] = b[k];
        k++;
        i++;
      }
    }
    else {
      for (; i < m + n;) {
        sorted[i] = a[j];
        j++;
        i++;
      }
    }
  }
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
