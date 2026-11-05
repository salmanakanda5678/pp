#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int A[2][2] = {{1,2},{3,4}};
    int B[2][2] = {{5,6},{7,8}};
    int row[2], result[2], C[2][2];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 2) {
        if (rank == 0) printf("Run with 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    /* Send rows of A */
    if (rank == 0) {
        row[0] = A[0][0];
        row[1] = A[0][1];

        result[0] = row[0]*B[0][0] + row[1]*B[1][0];
        result[1] = row[0]*B[0][1] + row[1]*B[1][1];

        C[0][0] = result[0];
        C[0][1] = result[1];

        MPI_Send(A[1], 2, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(row, 2, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        result[0] = row[0]*B[0][0] + row[1]*B[1][0];
        result[1] = row[0]*B[0][1] + row[1]*B[1][1];

        MPI_Send(result, 2, MPI_INT, 0, 1,
                 MPI_COMM_WORLD);
    }

    if (rank == 0) {
        MPI_Recv(C[1], 2, MPI_INT, 1, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Matrix C = A x B\n");
        printf("%d %d\n", C[0][0], C[0][1]);
        printf("%d %d\n", C[1][0], C[1][1]);
    }

    MPI_Finalize();
    return 0;
}
