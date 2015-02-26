#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int ** alloc_matrix(int n, int m); 
void init_matrix(int n, int m, int ** a);
void init_matrix1(int n, int m, int ** a);
void print_matrix(int n, int m, int ** a);

int ** prod_matrix(int n, int l, int m, int ** a, int ** b); //Computes product - (n, l, m) -> size of matrix
int ** trans_matrix(int n, int m, int ** a);

int main(int argc, char ** argv){
        int size, rank, tag=1, i,j,  n=12000, **a, **b, **c, **a1, **c1;
	double time1, totalTime;
	MPI_Status stat;
	MPI_Datatype rowtype;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Type_contiguous(n, MPI_INT, &rowtype);
	MPI_Type_commit(&rowtype);

	a=alloc_matrix(n,n);
	b=alloc_matrix(n,n);
	c=alloc_matrix(n,n);

	a1=alloc_matrix(n/size,n);
	c1=alloc_matrix(n/size,n);

	if (rank == 0) {

		// initialise the matrices

		init_matrix(n,n,a);
		init_matrix1(n,n,b);
		
		//printf("Matrix A:\n\n");
		//print_matrix(n,n,a);

		//printf("Matrix B:\n\n");
                //print_matrix(n,n,b);

	}
	
	time1 = MPI_Wtime();
	//Scatter A and Bcast B to all
	MPI_Scatter(&a[0][0], n*n/size, MPI_INT, &a1[0][0], n*n/size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&b[0][0], n*n, MPI_INT, 0, MPI_COMM_WORLD);
	//Compute product c1 = a1*b
	c1 = prod_matrix(n/size, n, n, a1, b);
	//Gather c1 back onto root (as c)
	MPI_Gather(&c1[0][0], n*n/size, MPI_INT, &c[0][0], n*n/size, MPI_INT, 0, MPI_COMM_WORLD);
	
	time1 = MPI_Wtime() - time1;
	MPI_Reduce(&time1, &totalTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	//Write elements of c (as root)
	if(rank == 0)
	{
	    //printf("\n\nMatrix c\n\n");
	    //print_matrix(n,n,c);

	    printf("\n\n%1f\n\n", totalTime);
	}

	MPI_Type_free(&rowtype);
	MPI_Finalize();
}


/*

The function alloc_matrix is to allocate dynamically a matrix of integers with
 n rows and m columns.

The function arguments are;

	n - number of rows
	m - number of columns

The function returns the double pointer representing the matrix.



*/

int ** alloc_matrix(int n, int m){

	int i, j, **a, *aa;

	aa=(int *) calloc(n*m, sizeof(int));
	a=(int **) calloc(n, sizeof(int*));

	for(i=0;i<n;i++)a[i]=aa+i*m;

	for(i=0;i<n;i++)for(j=0;j<m;j++)a[i][j]=0;

	return a;
}


/*

The function init_matrix is to initialize the matrix a with random integers.

The function arguments are:

	n - number of rows
	m - number of columns
	a - the matrix


*/



void init_matrix(int n, int m, int ** a){

	int i, j;

	for(i=0;i<n;i++)for(j=0;j<m;j++)a[i][j]=rand()%100;


}

void init_matrix1(int n, int m, int ** a){

        int i, j;

        for(i=0;i<n;i++)for(j=0;j<m;j++)a[i][j]= (i == j)?1:0;


}



/*

The function print_matrix is to display the matrix a.

The function arguments are:

	n - number of rows
	m - number of columns
	a - the matrix


*/



void print_matrix(int n, int m, int ** a){

	int i, j;

	printf("\n");

	for(i=0;i<n;i++){

		for(j=0;j<m;j++)printf("%d ",a[i][j]);

		printf("\n");

	}


}


/*

The function prod_matrix is to multiply the matrices a and b.

The function arguments are;

	n - number of rows of a
	l - number of columns of a // the matrix b must have l rows
	m - number of columns of b

	a - the first matrix
	b - the second matrix

The function returns the double pointer representing the product matrix.



*/


int ** prod_matrix(int n, int l, int m, int ** a, int ** b){

	int i,j,k,** c;

	c=alloc_matrix(n,m);

	for(i=0;i<n;i++)for(j=0;j<m;j++){

		c[i][j]=0;

		for(k=0;k<l;k++){
			//c[i][j]=c[i][j]+a[i][k]*b[k][j];
			c[i][j]=c[i][j]+a[i][k]*b[j][k];

		}

	}

	return c;

}


int ** trans_matrix(int n, int m, int ** a){

        int i,j;
	int ** b;

	b=alloc_matrix(m,n);

	for(j=0;j<m;j++)for(i=0;i<n;i++){

		b[j][i]=a[i][j];

	}

	return b;

}
