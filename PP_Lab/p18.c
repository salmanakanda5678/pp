#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, local[4], hist[10] = {0};
    int global[10];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int data[4];

    if(rank == 0) {
        int x[] = {1,2,2,3};
        for(int i=0;i<4;i++) data[i] = x[i];
    }
    else if(rank == 1) {
        int x[] = {2,3,4,4};
        for(int i=0;i<4;i++) data[i] = x[i];
    }
    else if(rank == 2) {
        int x[] = {1,4,5,5};
        for(int i=0;i<4;i++) data[i] = x[i];
    }
    else {
        int x[] = {0,2,5,9};
        for(int i=0;i<4;i++) data[i] = x[i];
    }

    for(int i=0;i<4;i++)
        hist[data[i]]++;

    MPI_Allreduce(hist, global, 10, MPI_INT,
                  MPI_SUM, MPI_COMM_WORLD);

    if(rank == 0) {
        printf("Global Histogram:\n");
        for(int i=0;i<10;i++)
            printf("%d : %d\n", i, global[i]);
    }

    MPI_Finalize();
    return 0;
}
