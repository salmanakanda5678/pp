#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, send_data, recv_data;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    send_data = (rank == 0) ? 100 : 200;

    if(rank == 0) {
        MPI_Send(&send_data,1,MPI_INT,1,0,MPI_COMM_WORLD);
        MPI_Recv(&recv_data,1,MPI_INT,1,0,MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
    }
    else {
        MPI_Recv(&recv_data,1,MPI_INT,0,0,MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        MPI_Send(&send_data,1,MPI_INT,0,0,MPI_COMM_WORLD);
    }

    printf("P%d received %d\n",rank,recv_data);

    MPI_Finalize();
    return 0;
}
