#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, send[4], recv[4];
    double t1, t2;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(size != 4) {
        if(rank == 0)
            printf("Run with 4 processes\n");
        MPI_Finalize();
        return 0;
    }

    for(int i=0; i<4; i++)
        send[i] = rank*10 + i;

    MPI_Barrier(MPI_COMM_WORLD);
    t1 = MPI_Wtime();

    MPI_Alltoall(send, 1, MPI_INT,
                 recv, 1, MPI_INT,
                 MPI_COMM_WORLD);

    t2 = MPI_Wtime();

    printf("P%d received: ", rank);
    for(int i=0; i<4; i++)
        printf("%d ", recv[i]);

    printf("\nP%d communication time = %f s\n",
           rank, t2-t1);

    MPI_Finalize();
    return 0;
}
