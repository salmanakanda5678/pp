#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_workers = size - 1;
    if (num_workers <= 0) {
        if (rank == 0) {
            printf("Error: This program requires at least 2 processes (1 Master + at least 1 Worker).\n");
        }
        MPI_Finalize();
        return 0;
    }

    int total_elements = 12;
    int elements_per_worker = total_elements / num_workers;
    int array[] = {12, 45, 23, 67, 34, 89, 56, 78, 91, 25, 43, 60};

    if (rank == 0) {
        printf("P0 distributed array elements to workers.\n");

        // Distribute chunks to each worker
        for (int i = 1; i < size; i++) {
            MPI_Send(&array[(i - 1) * elements_per_worker], elements_per_worker, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Receive maximums from workers and find the overall maximum
        int overall_max = -999999;
        for (int i = 1; i < size; i++) {
            int local_max;
            MPI_Recv(&local_max, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("P%d found maximum: %d\n", i, local_max);
            printf("P%d sent maximum: %d to P0\n", i, local_max);
            
            if (local_max > overall_max) {
                overall_max = local_max;
            }
        }

        printf("P0 received maximum values from all workers.\n");
        printf("Overall maximum element = %d\n", overall_max);

    } else {
        int *local_array = (int*)malloc(elements_per_worker * sizeof(int));
        
        // Receive chunk from Master
        MPI_Recv(local_array, elements_per_worker, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Find local maximum
        int local_max = local_array[0];
        for (int i = 1; i < elements_per_worker; i++) {
            if (local_array[i] > local_max) {
                local_max = local_array[i];
            }
        }

        // Send local maximum back to Master
        MPI_Send(&local_max, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        free(local_array);
    }

    MPI_Finalize();
    return 0;
}