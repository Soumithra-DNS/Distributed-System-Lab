/*
Problem_16: Each MPI process stores a local value. Use MPI_Scan to calculate the prefix sum across all processes and compare it with a manually implemented point-to-point solution. 
Sample Input
Number of processes: 4
P0 = 10
P1 = 20
P2 = 30
P3 = 40
Sample Output
P0: Local value = 10, Prefix sum = 10
P1: Local value = 20, Prefix sum = 30
P2: Local value = 30, Prefix sum = 60
P3: Local value = 40, Prefix sum = 100

Prefix result: 10 30 60 100
*/

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
