/* (c) 1996,1997 Peter Sanders, Ingo Boesnach */
/* simulate a cellular automaton (serial version)
 * periodic boundaries
 * 
 * #1: Number of lines 
 * #2: Number of iterations to be simulated
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "random.h"
#include "md5tool.h"

#include "mpi.h"

/* horizontal size of the configuration */
#define XSIZE 1024

/* berechnet wieviel Zeilen jeder Prozess uebernehmen soll */
#define DIVIDE(lines, size, rank) \
    ( (lines) / ((size)-1) + ( (rank)==(size)-1 ? (lines) % ((size)-1) : 0 ) )

/* "ADT" State and line of states (plus border) */
typedef char State;
typedef State Line[XSIZE + 2];

/* determine random integer between 0 and n-1 */
#define randInt(n) ((int)(nextRandomLEcuyer() * n))

/* --------------------- CA simulation -------------------------------- */

/* random starting configuration */
static void initConfig(Line *buf, int lines)
{  int x, y;

   initRandomLEcuyer(424243);
   for (y = 1;  y <= lines;  y++) {
      for (x = 1;  x <= XSIZE;  x++) {
         buf[y][x] = randInt(100) >= 50;
      }
   }
}

/* annealing rule from ChoDro96 page 34 
 * the table is used to map the number of nonzero
 * states in the neighborhood to the new state
 */
static State anneal[10] = {0, 0, 0, 0, 1, 0, 1, 1, 1, 1};

/* a: pointer to array; x,y: coordinates; result: n-th element of anneal,
      where n is the number of neighbors */
#define transition(a, x, y) \
   (anneal[(a)[(y)-1][(x)-1] + (a)[(y)][(x)-1] + (a)[(y)+1][(x)-1] +\
           (a)[(y)-1][(x)  ] + (a)[(y)][(x)  ] + (a)[(y)+1][(x)  ] +\
           (a)[(y)-1][(x)+1] + (a)[(y)][(x)+1] + (a)[(y)+1][(x)+1]])

/* treat torus like boundary conditions */
static void boundary(Line *buf, int lines, int rank, int size)
{  
    int x,y;
    MPI_Status status;
    MPI_Request request[2];

    for (y = 0;  y <= lines+1;  y++) {
        /* copy rightmost column to the buffer column 0 */
        buf[y][0      ] = buf[y][XSIZE];

        /* copy leftmost column to the buffer column XSIZE + 1 */
        buf[y][XSIZE+1] = buf[y][1    ];
    }

    /* sende letzte zeile in die erste buffer zeile des Nachfolgers */
    MPI_Isend(
        buf[ lines ],
        XSIZE + 2,
        MPI_CHAR,
        rank == (size - 1) ? 1 : rank + 1,
        1,
        MPI_COMM_WORLD,
        &request[0]
    );

    /* sende erste zeile in die letzte buffer zeile des Vorgaengers */
    MPI_Isend(
        buf[ 1 ],
        XSIZE + 2,
        MPI_CHAR,
        rank == 1 ? size - 1 : rank - 1,
        1,
        MPI_COMM_WORLD,
        &request[1]
    );

    /* empfange erste Zeile des Nachfolgers und speichere in die letzte Zeile */
    MPI_Recv(
        buf[ lines + 1 ],
        XSIZE + 2,
        MPI_CHAR,
        rank == (size - 1) ? 1 : rank + 1,
        1,
        MPI_COMM_WORLD,
        &status
    );


    /* empfange letzte Zeile des Vorgaengers und speichere in die erste Zeile */
    MPI_Recv(
        buf[ 0 ],
        XSIZE + 2,
        MPI_CHAR,
        rank == 1 ? size - 1 : rank - 1,
        1,
        MPI_COMM_WORLD,
        &status
    );

}

/* make one simulation iteration with lines lines.
 * old configuration is in from, new one is written to to.
 */
static void simulate(Line *from, Line *to, int lines, int rank, int size)
{
    int x, y;

    boundary(from, lines, rank, size);
    for (y = 1;  y <= lines;  y++) {
        for (x = 1;  x <= XSIZE;  x++) {
            to[y][x  ] = transition(from, x  , y);
        }
    }
}


/* --------------------- measurement ---------------------------------- */

int main(int argc, char** argv)
{  
    int lines, its;
    int i;
    Line *from, *to, *temp;
    char* hash;

    int rank, size;
    MPI_Status status;

    MPI_Init( &argc, &argv );

    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );

    assert(argc == 3);

    lines = atoi(argv[1]);
    its   = atoi(argv[2]);
    
    if( rank == 0 )
    {
        from = malloc((lines + 2) * sizeof(Line));

        initConfig(from, lines);

        /* sende ausgangslage an alle worker */
        for( i = 1 ; i < size ; i++ )
        {
            MPI_Send(
                from[ 1 + (i-1) * (lines / (size-1)) ],
                DIVIDE(lines, size, i) * sizeof(Line),
                MPI_CHAR,
                i,
                0,
                MPI_COMM_WORLD
            );
        }

        /* empfange resultate von den workern */
        for( i = 1 ; i < size ; i++ )
        {
            MPI_Recv(
                from[ 1 + (i-1) * (lines / (size-1)) ],
                DIVIDE(lines, size, i) * sizeof(Line),
                MPI_CHAR,
                i,
                0,
                MPI_COMM_WORLD,
                &status
            );
        }

        hash = getMD5DigestStr(from[1], sizeof(Line) * (lines));
        printf("hash: %s\n", hash);

        free(from);
        free(hash);
    }else{
        from = malloc( ( DIVIDE( lines, size, rank ) + 2 ) * sizeof( Line ) );
        to   = malloc( ( DIVIDE( lines, size, rank ) + 2 ) * sizeof( Line ) );
        
        /* empfange ausgangslage */
        MPI_Recv(
            &from[1],
            DIVIDE(lines, size, rank) * sizeof(Line),
            MPI_CHAR,
            0,
            0,
            MPI_COMM_WORLD,
            &status
        );

        /* berechne ergebnis */
        for( i = 0 ; i < its ; i++ )
        {
            simulate(from, to, DIVIDE(lines, size, rank), rank, size);

            temp = from ;
            from = to   ;
            to   = temp ;
        }

        /* sende ergebnis */
        MPI_Send(
            &from[1],
            DIVIDE(lines, size, rank) * sizeof(Line),
            MPI_CHAR,
            0,
            0,
            MPI_COMM_WORLD
        );
        
        free(from);
        free(to);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
