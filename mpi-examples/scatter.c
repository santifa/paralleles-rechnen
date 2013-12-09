#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

#define SEND_SIZE 4
 
int main(int argc, char *argv[])
{
    int rank, size, tag=0, i, recv;
    MPI_Status status;
 
    int send_buffer[ SEND_SIZE ][ SEND_SIZE ] = {
        {  1,  2,  3,  4 },
        {  5,  6,  7,  8 },
        {  9, 10, 11, 12 },
        { 13, 14, 15, 16 }
    };

    int recv_buffer[ SEND_SIZE ];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
 
    if ( size == SEND_SIZE ) 
    {
        MPI_Scatter(
            send_buffer,
            SEND_SIZE,
            MPI_INT,
            recv_buffer,
            SEND_SIZE,
            MPI_INT,
            0,
            MPI_COMM_WORLD
        );



        printf( 
            "rank=%d, summe : %d \n", 
            rank, 
            recv_buffer[0] + 
            recv_buffer[1] + 
            recv_buffer[2] +
            recv_buffer[3]
        );
    }
    else 
    {
        printf("es muessen 4 prozesse sein : mpirun -np 4\n");
    }


    MPI_Finalize();
    return EXIT_SUCCESS;
}




