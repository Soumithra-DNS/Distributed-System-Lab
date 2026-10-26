/*
Problem_13: Distribute matrix rows among processes when the number of rows is not evenly divisible by the number of processes. Collect the processed rows using MPI_Gatherv. 
Sample Input
Number of processes: 3
Matrix:
1  2  3
4  5  6
7  8  9
10 11 12
13 14 15

Five rows are distributed as:
P0 → 2 rows
P1 → 2 rows
P2 → 1 row

Operation: multiply every element by 2.
Sample Output
P0:
2 4 6
8 10 12
P1:
14 16 18
20 22 24
P2:
26 28 30
Final matrix:
2 4 6
8 10 12
14 16 18
20 22 24
26 28 30
*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int A[15] = {1,2,3, 4,5,6, 7,8,9,
                 10,11,12, 13,14,15};
    int rows[3] = {2,2,1};
    int sendcnt[3] = {6,6,3};
    int displs[3] = {0,6,12};
    int local[6], result[15];

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    if(size != 3) {
        if(rank==0) printf("Run with 3 processes\n");
        MPI_Finalize();
        return 0;
    }

    int n = rows[rank] * 3;

    MPI_Scatterv(A,sendcnt,displs,MPI_INT,
                 local,n,MPI_INT,0,MPI_COMM_WORLD);

    for(int i=0;i<n;i++)
        local[i] *= 2;

    MPI_Gatherv(local,n,MPI_INT,
                result,sendcnt,displs,MPI_INT,
                0,MPI_COMM_WORLD);

    if(rank==0) {
        printf("Result:\n");
        for(int i=0;i<15;i++) {
            printf("%d ",result[i]);
            if((i+1)%3==0) printf("\n");
        }
    }

    MPI_Finalize();
    return 0;
}
