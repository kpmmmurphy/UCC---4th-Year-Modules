//mperun -np 4 myPiExample

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#include "mpe.h"
#include "mpe_graphics.h"

int distance(x1,y1,x2,y2){

	return sqrt(pow(x2-x1,2) + pow(y2-y1,2.0));
}


int main( int argc, char** argv )
{
	// MPI size, rank variables
	int size, rank;


	// MPI Wtime variables
	double my_time, max_time;

	// MPE Variables
	MPE_XGraph graph;
	MPE_Color my_color;
	MPE_Color * colors;
	MPE_Point * points , * point;

	// Window variables
	int width = 600, length = 20;

	// Circle and point variables
	//int centerX = (int)width/2;
	//int centerY = (int)width/2;
	//int radius = (int)width/2;
	//int x,y;

	// Value for n - how many random points are generated
	int n=1000;

	// Misc variables
	int i,x,y, x1, y1, dir;

	// Point counters
	int interCount=0,mainInterCount;

	// Pi variable
	double piValue=0.0;

	// Start of program
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPE_Open_graphics( &graph, MPI_COMM_WORLD, NULL,-1, -1, width, width, 0 );

	// create a colour for each processor
	colors = (MPE_Color *) malloc((size+1) * sizeof(MPE_Color));
	MPE_Make_color_array (graph, size+1, colors);


	if(rank==0){
		//MPE_Draw_circle(graph,centerX,centerY,radius,colors[0]);
		//MPE_Update(graph);
		//Draw some || lines
		for(i=0;i*length<width;i++)
		{
		    MPE_Draw_line(graph,0, i*length, width, i*length, colors[0]);
		}
	}

	my_time = MPI_Wtime();

	srand((unsigned)time(NULL)*(rank+1));

	for (i =0; i <n; i++){
		//Needle eye
		x = rand()%width;
		y = rand()%width;

		dir = rand()%360;

		//Needle top
		x1 = x + (int) length*cos(dir * 3.1415/180);
		y1 = y + (int) length*sin(dir * 3.1415/180);

		MPE_Draw_circle(graph, x, y, 2, colors[rank+1]);
		MPE_Draw_line(graph, x, y, x1, y1, colors[rank+1]);
		MPE_Update(graph);

		//Test intersections
		if (y/length != y1/length) //distance(centerX,centerY,x,y) < radius)
        	{
		    interCount++;
		    //MPE_Fill_circle(graph,x,y,1,colors[rank+1]);
		    //MPE_Update( graph );
        	}
	}

	my_time = MPI_Wtime()- my_time;

	printf("%lf Time taken for process %d is %lf\n",MPI_Wtime(),rank,my_time);
	fflush(stdout);

	MPI_Reduce(&interCount,&mainInterCount,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	if ( rank == 0 )
	{
		char ckey;
		piValue = (2.0*size*n)/mainInterCount;
		printf("PI Estimate is : %lf\n", piValue);
		printf( "Hit any key then return to continue  " );
		scanf( "%c", &ckey );printf( "\n" );
	}

	MPE_Close_graphics( &graph );
	MPI_Finalize();

	return 0;
}
