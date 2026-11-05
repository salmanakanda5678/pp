#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // স্যাম্পল ইনপুট অনুযায়ী লোকাল ভ্যালু ডিফাইন করা হলো
    int local_values[] = {45, 30, 50, 20, 35};
    int local_val = 0;

    if (rank < size) {
        local_val = local_values[rank];
    }

    int prefix_min = 0;

    // MPI_Scan ব্যবহার করে প্রিফিক্স মিনিমাম হিসাব করা হচ্ছে
    MPI_Scan(&local_val, &prefix_min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);

    // সব প্রসেসের লোকাল ভ্যালু এবং প্রিফিক্স মিনিমামগুলো P0 তে সংগ্রহ করার জন্য অ্যারে
    int *all_local_vals = NULL;
    int *all_prefix_mins = NULL;

    if (rank == 0) {
        all_local_vals = (int*)malloc(size * sizeof(int));
        all_prefix_mins = (int*)malloc(size * sizeof(int));
    }

    // সব প্রসেসের ডাটা P0 তে Gather করা হচ্ছে
    MPI_Gather(&local_val, 1, MPI_INT, all_local_vals, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&prefix_min, 1, MPI_INT, all_prefix_mins, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // এখন শুধু P0 প্রসেস সিকোয়েন্সিয়ালি প্রিন্ট করবে, যেন আউটপুট এলোমেলো না হয়
    if (rank == 0) {
        printf("P0 distributed elements to workers.\n");
        for (int i = 0; i < size; i++) {
            printf("P%d: Local value = %d, Prefix minimum = %d\n", i, all_local_vals[i], all_prefix_mins[i]);
        }
        printf("Prefix minimum result:");
        for (int i = 0; i < size; i++) {
            printf(" %d", all_prefix_mins[i]);
        }
        printf("\n");

        free(all_local_vals);
        free(all_prefix_mins);
    }

    MPI_Finalize();
    return 0;
}