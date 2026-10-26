/*
Problem_14: Each process generates a set of numerical values. Use MPI_Reduce to calculate the global sum, maximum, minimum, and average.
Sample Input
Number of processes: 4
P0: 10 20 30
P1: 40 50 60
P2: 70 80 90
P3: 100 110 120
Sample Output
Local sums:
P0 = 60
P1 = 150
P2 = 240
P3 = 330
Global Sum     	= 780
Global Maximum 	= 120
Global Minimum	= 10
Average        		= 65.00
*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, local[3];
    int sum = 0, total, max, min;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for(int i=0;i<3;i++)
        local[i] = rank * 30 + (i+1)*10;

    for(int i=0;i<3;i++)
        sum += local[i];

    int local_max = local[2];
    int local_min = local[0];

    MPI_Reduce(&sum,&total,1,MPI_INT,MPI_SUM,
               0,MPI_COMM_WORLD);

    MPI_Reduce(&local_max,&max,1,MPI_INT,MPI_MAX,
               0,MPI_COMM_WORLD);

    MPI_Reduce(&local_min,&min,1,MPI_INT,MPI_MIN,
               0,MPI_COMM_WORLD);

    if(rank==0) {
        printf("Global Sum = %d\n",total);
        printf("Global Maximum = %d\n",max);
        printf("Global Minimum = %d\n",min);
        printf("Average = %.2f\n",total/12.0);
    }

    MPI_Finalize();
    return 0;
}
