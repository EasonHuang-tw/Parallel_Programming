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

    // TODO: binary tree redunction

    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    unsigned int seed = time(NULL)*(world_rank+1);
    double y;
    double x;
    long long sum =0;
    long long it = tosses/world_size;

        // TODO: handle workers
    for(int i = 0; i < it  ;i++){
            x = (double)rand_r(&seed)/RAND_MAX;
            y = (double)rand_r(&seed)/RAND_MAX;
            if ( x*x + y*y <= 1.0)
                sum++;
        }
	long long data = 0;
	int j =1;
    for(int i = 1 ; ; i*=2){
		if( world_rank % (i*2) != 0 ){
			MPI_Send(&sum, 1, MPI_LONG_LONG, world_rank - i , 0, MPI_COMM_WORLD);
			break;
		}
		else{
			if( (world_rank + i) <= (world_size - 1)){
				MPI_Recv( &data, 1, MPI_LONG_LONG, world_rank + i , 0,  MPI_COMM_WORLD, MPI_STATUS_IGNORE);	
				sum += data;
			}
			else{
				break;
			}
		}
		j++;

	}


    if (world_rank == 0)
    {
        // TODO: PI result
		
		pi_result = 4.0 * (double)sum / (double)tosses;

        // --- DON'T TOUCH ---
        double end_time = MPI_Wtime();
        printf("%lf\n", pi_result);
        printf("MPI running time: %lf Seconds\n", end_time - start_time);
        // ---
    }

    MPI_Finalize();
    return 0;
}
