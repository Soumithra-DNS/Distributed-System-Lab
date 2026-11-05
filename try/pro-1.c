
// #include <stdio.h>
// #include <mpi.h>

// int main(int argc, char *argv[])
// {
//     int rank, size;
//     int n, chunk;
//     int array[100];
//     int local[100];
//     int local_max, overall_max;

//     MPI_Init(&argc, &argv);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//     MPI_Comm_size(MPI_COMM_WORLD, &size);

//     if (rank == 0)
//     {
//         FILE *in = fopen("input1.txt", "r");

//         if (in == NULL)
//         {
//             printf("Cannot open input1.txt\n");
//             MPI_Abort(MPI_COMM_WORLD, 1);
//         }

//         fscanf(in, "%d", &n);

//         for (int i = 0; i < n; i++)
//             fscanf(in, "%d", &array[i]);

//         fclose(in);

//         if (size < 2 || n % (size - 1) != 0)
//         {
//             printf("Invalid process count or array size.\n");
//             MPI_Abort(MPI_COMM_WORLD, 1);
//         }

//         chunk = n / (size - 1);

//         printf("P0 distributed array elements to workers.\n");

//         for (int i = 1; i < size; i++)
//         {
//             MPI_Send(&chunk, 1, MPI_INT, i, 0,
//                      MPI_COMM_WORLD);

//             MPI_Send(&array[(i - 1) * chunk], chunk,
//                      MPI_INT, i, 1, MPI_COMM_WORLD);
//         }

//         FILE *out = fopen("output1.txt", "w");

//         if (out == NULL)
//         {
//             printf("Cannot create output1.txt\n");
//             MPI_Abort(MPI_COMM_WORLD, 1);
//         }

//         fprintf(out, "P0 distributed array elements to workers.\n\n");

//         overall_max = -2147483647 - 1;

//         for (int i = 1; i < size; i++)
//         {
//             MPI_Recv(&local_max, 1, MPI_INT, i, 2,
//                      MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//             fprintf(out, "P%d found maximum: %d\n",
//                     i, local_max);

//             fprintf(out, "P%d sent maximum: %d to P0\n",
//                     i, local_max);

//             if (local_max > overall_max)
//                 overall_max = local_max;
//         }

//         fprintf(out, "\nP0 received maximum values from all workers.\n");
//         fprintf(out, "Overall maximum element = %d\n", overall_max);

//         fclose(out);

//         printf("Overall maximum element = %d\n", overall_max);
//         printf("Result saved in output1.txt\n");
//     }
//     else
//     {
//         MPI_Recv(&chunk, 1, MPI_INT, 0, 0,
//                  MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//         MPI_Recv(local, chunk, MPI_INT, 0, 1,
//                  MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//         local_max = local[0];

//         for (int i = 1; i < chunk; i++)
//         {
//             if (local[i] > local_max)
//                 local_max = local[i];
//         }

//         MPI_Send(&local_max, 1, MPI_INT, 0, 2,
//                  MPI_COMM_WORLD);
//     }

//     MPI_Finalize();
//     return 0;
// }






#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int n = 12, chunk;
    int array[12] = {12, 45, 23, 67, 34, 89,
                     56, 78, 91, 25, 43, 60};
    int local[100];
    int local_max, overall_max;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2 || n % (size - 1) != 0)
    {
        if (rank == 0)
            printf("Invalid process count or array size.\n");

        MPI_Finalize();
        return 0;
    }

    if (rank == 0)
    {
        chunk = n / (size - 1);

        printf("P0 distributed array elements to workers.\n\n");

        // Distribute array elements
        for (int i = 1; i < size; i++)
        {
            MPI_Send(&chunk, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            MPI_Send(&array[(i - 1) * chunk], chunk,
                     MPI_INT, i, 1, MPI_COMM_WORLD);
        }

        // Receive maximum values from workers
        overall_max = -2147483647 - 1;

        for (int i = 1; i < size; i++)
        {
            MPI_Recv(&local_max, 1, MPI_INT, i, 2,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            printf("P%d found maximum: %d\n", i, local_max);
            printf("P%d sent maximum: %d to P0\n", i, local_max);

            if (local_max > overall_max)
                overall_max = local_max;
        }

        printf("\nP0 received maximum values from all workers.\n");
        printf("Overall maximum element = %d\n", overall_max);
    }
    else
    {
        // Receive assigned elements
        MPI_Recv(&chunk, 1, MPI_INT, 0, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Recv(local, chunk, MPI_INT, 0, 1,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Find local maximum
        local_max = local[0];

        for (int i = 1; i < chunk; i++)
        {
            if (local[i] > local_max)
                local_max = local[i];
        }

        // Send maximum to Master
        MPI_Send(&local_max, 1, MPI_INT, 0, 2,
                 MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
