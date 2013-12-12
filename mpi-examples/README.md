MPI_Gather
=====

Jeder Prozess schickt den Inhalt seines Send_Buffers zu
Root Prozess, welcher alles Daten in der Reihenfolge der
Ranks empfängt und sortiert.

```
#include <mpi.h>
int MPI_Gather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
    void *recvbuf, int recvcount, MPI_Datatype recvtype, int root
    MPI_comm comm)

sendbuf   - Startadresse des Sendepuffers
sendcount - Anzahl der zu sendenden Elemente
sendtype  - Datentyp der zu sendenden Elemente
recvbuf   - Startadresse des Empfangspuffers
recvcount - Anzahl der zu empfangenden Elemente pro Prozess
recvtype  - Datentyp der zu empfangenden Elemente
root      - Rang des Masterprozesses
comm      - Kommunikator-Handle
```

MPI_Gather Nachbildung
=====

Eine Nachbildung der MPI_Gather Funktion mithilfe
der MPI_Send, MPI_Recv, MPI_Comm_rank, MPI_Comm_size Funktionen.

```
/* Setting up MPI */
MPI_Comm comm;
int i, root, myrank, *rbuf, gsize;

MPI_Comm_rank(comm, myrank);

  if (myrank = root) {
    int src;
    MPI_Comm_size(comm, &gsize);
    rbuf = (int *) malloc (gsize * sizeOf(int));

    for (i = 0; i <= gsize; i++) {
      MPI_Recv(rbuf + i, 1, int, src, 0, comm)
    }
  } else {
    int *buf;
    /* irgendwelche operationen */
    MPI_Send(buf, 1, int, root, 0, comm);
  }
}

```
