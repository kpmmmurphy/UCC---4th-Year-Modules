
//Include Headers
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "mpi.h"

//Function Definitions
int rps_test(int move1, int move2);
char *moves = "rps";

int main(int argc, char **argv)
{
	//Declarations
	int rank, size;
	int move_id = 0, other_move_id = 0, tag1 = 1, tag2 = 2;
	unsigned int iseed = (unsigned int)time(NULL);
	int result;
	MPI_Status status;

	//Init the MPI world and get rank and size
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//I am processor rank. What am I doing?

	//Aquire local data () -- Will be different accross machines
	srand(iseed*rank);       // seed the generator with a different seed.
	move_id=rand()%3;    // generate a random number between 0-2.

	//Process the data
	if(rank == 0)
	{
		//Ping-pong + test
		MPI_Send(&move_id,       1, MPI_INT,1, tag1, MPI_COMM_WORLD);
		MPI_Recv(&other_move_id, 1, MPI_INT,1, tag2, MPI_COMM_WORLD, &status);

		result = rps_test(move_id, other_move_id);
	}
	else if(rank == 1)
	{
		MPI_Recv(&other_move_id, 0, MPI_INT, 0, tag2, MPI_COMM_WORLD, &status);
		MPI_Send(&move_id,       1, MPI_INT, 0, tag1, MPI_COMM_WORLD);
		result = rps_test(other_move_id, move_id);
	}

	printf("Processor %d finds the winner %d \n", rank, result);
	//Terminate
	MPI_Finalize();
	return 1;
}

//Function Declarations
int rps_test(int move1, int move2)
{
	if((moves[move1]=='r')&&(moves[move2]=='s')||
	   (moves[move1]=='p')&&(moves[move2]=='r')||
	   (moves[move1]=='s')&&(moves[move2]=='p'))return 0;  //First Player wins
	else if((moves[move1]=='r')&&(moves[move2]=='p')||
	        (moves[move1]=='p')&&(moves[move2]=='s')||
	        (moves[move1]=='s')&&(moves[move2]=='r')) return 1;  //Second Player wins
	else return -1;  //Draw
}


