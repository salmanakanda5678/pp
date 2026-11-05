#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size, row[4], sum = 0, total = 0;
    int matrix[4][4] = {
        {1,2,3,4},
        {5,6,7,8},
        {9,10,11,12},
        {13,14,15,16}
    };

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            printf("Run with 4 processes.\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        for (int i = 0; i < 4; i++) {
            if (i == 0) {
                for (int j = 0; j < 4; j++)
                    row[j] = matrix[0][j];
            } else {
                MPI_Send(matrix[i], 4, MPI_INT, i, 0,
                         MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(row, 4, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < 4; i++)
        sum += row[i];

    printf("P%d: Row = %d %d %d %d Sum = %d\n",
           rank, row[0], row[1], row[2], row[3], sum);

    if (rank != 0)
        MPI_Send(&sum, 1, MPI_INT, 0, 1,
                 MPI_COMM_WORLD);
    else {
        total = sum;
        for (int i = 1; i < 4; i++) {
            int s;
            MPI_Recv(&s, 1, MPI_INT, i, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += s;
        }
        printf("Total matrix sum = %d\n", total);
    }

    MPI_Finalize();
    return 0;
}
