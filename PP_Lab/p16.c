#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, value, prefix;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    value = (rank + 1) * 10;

    MPI_Scan(&value, &prefix, 1, MPI_INT,
             MPI_SUM, MPI_COMM_WORLD);

    printf("P%d: Value=%d, Prefix Sum=%d\n",
           rank, value, prefix);

    MPI_Finalize();
    return 0;
}
