#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    // TODO: MPI init

    // TODO: use MPI_Reduce

 	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
 	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    unsigned int seed = time(NULL)*(world_rank+1);
	double y;
	double x;
	long long sum =0,data=0;
	long long it = tosses/world_size;
	long long *recv;    
    

    for(int i = 0; i < it  ;i++){
        	x = (double)rand_r(&seed)/RAND_MAX;
        	y = (double)rand_r(&seed)/RAND_MAX;
        	if ( x*x + y*y <= 1.0)
            	sum++;
    }

	MPI_Reduce(&sum,&data ,1,MPI_LONG_LONG,MPI_SUM,0,MPI_COMM_WORLD);
	
	if (world_rank == 0)
    {
        // TODO: PI result

		pi_result = 4.0 * (double)data / (double)tosses;
        // --- DON'T TOUCH 
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
