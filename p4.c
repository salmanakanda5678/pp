#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int msg_size = 4*1024, iterations = 5;
    char *buffer;
    double start, end, round_trip, one_way;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0)
            printf("Run with 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    buffer = malloc(msg_size);

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    for (int i = 0; i < iterations; i++) {
        if (rank == 0) {
            MPI_Send(buffer, msg_size, MPI_CHAR, 1, 0,
                     MPI_COMM_WORLD);
            MPI_Recv(buffer, msg_size, MPI_CHAR, 1, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(buffer, msg_size, MPI_CHAR, 0, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(buffer, msg_size, MPI_CHAR, 0, 0,
                     MPI_COMM_WORLD);
        }
    }

    end = MPI_Wtime();

    if (rank == 0) {
        round_trip = (end - start) / iterations;
        one_way = round_trip / 2;

        printf("Message size : %d bytes\n", msg_size);
        printf("Iterations : %d\n", iterations);
        printf("Average round-trip : %f seconds\n", round_trip);
        printf("Average one-way : %f seconds\n", one_way);
    }

    free(buffer);
    MPI_Finalize();

    return 0;
}
