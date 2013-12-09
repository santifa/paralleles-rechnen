#include <stdio.h>
#include <string.h>
#include "mpi.h"

#define MESSAGE_SIZE 50
 
int main(int argc, char *argv[])
{
    int rank, size, tag=0, i;
    MPI_Status status;

    char message[ MESSAGE_SIZE ];
 
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
 
    if (rank == 0) {
        for( i = 1 ; i < size ; i++ ){
            MPI_Recv( 
                message, 
                MESSAGE_SIZE, 
                MPI_CHAR, 
                MPI_ANY_SOURCE, 
                tag, 
                MPI_COMM_WORLD, 
                &status
            );
            
            printf("empfangen von %d : %s\n", status.MPI_SOURCE, message);
        }

    }
    else {
        strcpy( message, "Hallo Welt");

        MPI_Send(
            message, 
            strlen( message ) + 1, 
            MPI_CHAR, 
            0, 
            tag, 
            MPI_COMM_WORLD
        );
    }

    MPI_Finalize();
    return 0;
}




