
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int values[5] = {45, 30, 50, 20, 35};
    int val, prefix;
    int manual, received;

    int scan_result[5], manual_result[5];
    int local_values[5];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 5)
    {
        if (rank == 0)
            printf("Run with 5 processes.\n");

        MPI_Finalize();
        return 0;
    }

    // Distribute one value to each process
    MPI_Scatter(values, 1, MPI_INT,
                &val, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Method 1: Using MPI_Scan
    MPI_Scan(&val, &prefix, 1, MPI_INT,
             MPI_MIN, MPI_COMM_WORLD);

    // Method 2: Manual point-to-point
    if (rank == 0)
    {
        manual = val;

        MPI_Send(&manual, 1, MPI_INT, 1, 0,
                 MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&received, 1, MPI_INT, rank - 1, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        manual = (val < received) ? val : received;

        if (rank < size - 1)
            MPI_Send(&manual, 1, MPI_INT, rank + 1, 0,
                     MPI_COMM_WORLD);
    }

    // Collect results at Master
    MPI_Gather(&val, 1, MPI_INT, local_values, 1,
               MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gather(&prefix, 1, MPI_INT, scan_result, 1,
               MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Gather(&manual, 1, MPI_INT, manual_result, 1,
               MPI_INT, 0, MPI_COMM_WORLD);

    // Display results
    if (rank == 0)
    {
        printf("P0 distributed elements to workers.\n\n");

        for (int i = 0; i < size; i++)
        {
            printf("P%d: Local value = %d, Prefix minimum = %d\n",
                   i, local_values[i], scan_result[i]);
        }

        printf("\nMPI_Scan result: ");

        for (int i = 0; i < size; i++)
            printf("%d ", scan_result[i]);

        printf("\nManual result:   ");

        for (int i = 0; i < size; i++)
            printf("%d ", manual_result[i]);

        int match = 1;

        for (int i = 0; i < size; i++)
        {
            if (scan_result[i] != manual_result[i])
                match = 0;
        }

        if (match)
            printf("\n\nBoth methods produce the same result.\n");
        else
            printf("\n\nResults do not match.\n");
    }

    MPI_Finalize();
    return 0;
}
