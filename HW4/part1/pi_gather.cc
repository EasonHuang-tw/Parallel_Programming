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

    // TODO: use MPI_Gather

 	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
 	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    unsigned int seed = time(NULL)*(world_rank+1);
	double y;
	double x;
	long long sum =0;
	long long it = tosses/world_size;
	long long *recv;    

    for(int i = 0; i < it  ;i++){
        	x = (double)rand_r(&seed)/RAND_MAX;
        	y = (double)rand_r(&seed)/RAND_MAX;
        	if ( x*x + y*y <= 1.0)
            	sum++;
    }
	if(world_rank==0){
		recv=(long long*)malloc(world_size*sizeof(long long));
	}

	MPI_Gather(&sum,1,MPI_LONG_LONG,recv,1,MPI_LONG_LONG,0,MPI_COMM_WORLD);

	if (world_rank == 0)
    {
        // TODO: PI result
		for(int i=1;i<world_size;i++){
			//printf("%lld\n",recv[i]);
			sum+=recv[i];
		}
		pi_result = 4.0 * (double)sum / (double)tosses;
        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // 
    }
    
    MPI_Finalize();
    return 0;
}
