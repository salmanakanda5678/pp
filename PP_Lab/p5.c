#include <stdio.h>
#include<stdlib.h>
#include <mpi.h>

#define MAX 100
#define MCW MPI_COMM_WORLD

int main(int argc, char *argv[])
{
    int rank, size;
    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int A[MAX][MAX], B[MAX][MAX], C[MAX][MAX];
    int row[MAX], result[MAX];
    FILE *file;
    int dims[4] = {0};

    if(rank==0){
        file = fopen("matrix.txt","r");
        fscanf(file,"%d %d %d %d",&dims[0], &dims[1],
                       &dims[2], &dims[3]);
    }
    MPI_Bcast(dims,4,MPI_INT,0,MCW);
    int rA = dims[0];
    int cA = dims[1];
    int rB = dims[2];
    int cB = dims[3];

    // printf("OK1");
    if(rank==0){
        for(int i=0;i<rA;i++){
            for (int j=0;j<cA;j++){
                fscanf(file,"%d",&A[i][j]);
                // printf("%d ",A[i][j]);
            }
            // printf("\n");
        }
        for(int i=0;i<rB;i++){
            for (int j=0;j<cB;j++){
                fscanf(file,"%d",&B[i][j]);
                //  printf("%d ",B[i][j]);
            }
            // printf("\n");
        }
        
        fclose(file);
    }
    
    /* Broadcast matrix B row by row */
    for (int i = 0; i < rB; i++) {
        MPI_Bcast(B[i], cB, MPI_INT, 0, MPI_COMM_WORLD);
    }
    // printf("OK2");
    
    if(rank==0){
        for(int p=1;p<size;p++){
            MPI_Send(A[p],cA,MPI_INT,p,10,MCW);
        }
        for(int j=0;j<cA;j++)
            row[j] = A[0][j];
    }
    else{
        MPI_Recv(row,cA,MPI_INT,0,10,MCW,MPI_STATUS_IGNORE);
    }

    for(int j=0;j<cB;j++){
        result[j] = 0;
        for(int k=0;k<cA;k++){
            result[j]+=row[k] * B[k][j];
        }
        // printf("Result %d \n",result[j]);
    }
    

    if(rank !=0){
        MPI_Send(result,cB,MPI_INT,0,20,MCW);
    }
    else{
        for(int j=0;j<cB;j++){
            C[0][j] = result[j];
        }

        for(int p=1;p<size;p++){
            MPI_Recv(C[p],cB,MPI_INT,p,20,MCW,MPI_STATUS_IGNORE);
        }
        printf("\nResult Matrix:\n");

        for(int i=0; i<rA; i++) {
            for(int j=0; j<cB; j++)
                printf("%d\t", C[i][j]);

            printf("\n");
        }
    }



    MPI_Finalize();
    return 0;
}