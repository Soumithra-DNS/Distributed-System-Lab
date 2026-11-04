/*
Problem_18: Divide a large dataset among MPI processes. Each process generates a local histogram, and MPI_Allreduce combines all local histograms into a global histogram. 
Sample Input
Number of processes: 4
Values range from 0 to 9.
P0: 1 2 2 3
P1: 2 3 4 4
P2: 1 4 5 5
P3: 0 2 5 9
Sample Output
Final histogram after MPI_Allreduce:
Value    Frequency
0        1
1        2
2        4
3        2
4        3
5        3
6        0
7        0
8        0
9        1

Total number of elements = 20
*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int hist[10] = {0};
    int global[10] = {0};
    
    int local_data[4];
    
    // Root process holds the full dataset (16 elements total, 4 per process)
    int global_data[16] = {
        1, 2, 2, 3,  // Data for P0
        2, 3, 4, 4,  // Data for P1
        1, 4, 5, 5,  // Data for P2
        0, 2, 5, 9   // Data for P3
    };

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure the program is run with exactly 4 processes
    if (size != 4) {
        if (rank == 0) {
            printf("Error: Please run this program with exactly 4 processes (-n 4).\n");
        }
        MPI_Finalize();
        return 0;
    }

    // Scatter 4 elements from global_data to each process's local_data
    MPI_Scatter(global_data, 4, MPI_INT, local_data, 4, MPI_INT, 0, MPI_COMM_WORLD);

    // Generate local histogram from received local data
    for(int i = 0; i < 4; i++) {
        hist[local_data[i]]++;
    }

    // Combine all local histograms into global histogram using MPI_Allreduce
    MPI_Allreduce(hist, global, 10, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // Rank 0 prints the final global histogram
    if(rank == 0) {
        printf("Final histogram after MPI_Allreduce:\n");
        printf("Value    Frequency\n");
        for(int i = 0; i < 10; i++) {
            printf("%d        %d\n", i, global[i]);
        }
    }

    MPI_Finalize();
    return 0;
}