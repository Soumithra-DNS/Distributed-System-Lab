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

#include <stdio.h>
#include <mpi.h>
int main(int argc, char *argv[])
{
    int rank, size, val, prefix;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4)
    {
        if (rank == 0)
            printf("Run with 4 processes.\n");
        MPI_Finalize();
        return 0;
    }

    val = (rank + 1) * 10;
    MPI_Scan(&val, &prefix, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    printf("P%d: Local value = %d, Prefix sum = %d\n", rank, val, prefix);

    int res[4];
    MPI_Gather(&prefix, 1, MPI_INT, res, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("\nPrefix result: ");
        for (int i = 0; i < 4; i++)
            printf("%d ", res[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}