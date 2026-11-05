#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int value = 100, received;
    int next, previous;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    next = (rank + 1) % size;
    previous = (rank - 1 + size) % size;

    start = MPI_Wtime();

    if (rank == 0)
        value = 100;

    MPI_Sendrecv(&value, 1, MPI_INT, next, 0,
                 &received, 1, MPI_INT, previous, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    end = MPI_Wtime();

    printf("P%d: Sent %d to P%d, Received %d from P%d\n",
           rank, value, next, received, previous);

    if (rank == 0)
        printf("Ring communication completed.\n");
        printf("Communication time = %f seconds\n", end - start);

    MPI_Finalize();
    return 0;
}
