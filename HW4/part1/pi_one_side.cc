#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

int fnz (long long  *schedule, long long *oldschedule, int size)
{
    static double starttime = -1.0;
    int diff = 0;

    for (int i = 1; i < size; i++){
//		printf("schedule : %lld old_schedule: %lld\n",schedule[i],oldschedule[i]);
  //     diff |= (schedule[i] != oldschedule[i]);
	//	if (diff&&schedule[i]!=0){
	//		oldschedule[i] = schedule[i];
	//	}
	
			if(schedule[i]==0){
				return 0;
			}
	}
	//else 
	return 1;
}

int main(int argc, char **argv)
{
    // --- DON'T TOUCH ---
    MPI_Init(&argc, &argv);
    double start_time = MPI_Wtime();
    double pi_result;
    long long int tosses = atoi(argv[1]);
    int world_rank, world_size;
    // ---

    MPI_Win win;

 	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
 	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    unsigned int seed = time(NULL)*(world_rank+1);
	double y;
	double x;
	long long sum =0;
	long long it = tosses/world_size;
    
	for(int i = 0; i < it  ;i++){
		x = (double)rand_r(&seed)/RAND_MAX;
		y = (double)rand_r(&seed)/RAND_MAX;
		if ( x*x + y*y <= 1.0)
			sum++;
	}
	// TODO: MPI init

    if (world_rank == 0)
    {
       long long *oldschedule =(long long*) malloc(world_size * sizeof(long long));
       // Use MPI to allocate memory for the target window
       long long *schedule;
       MPI_Alloc_mem(world_size * sizeof(long long), MPI_INFO_NULL, &schedule);

       for (int i = 0; i < world_size; i++)
       {
          schedule[i] = 0;
          oldschedule[i] = -1;
       }

       // Create a window. Set the displacement unit to sizeof(int) to simplify
       // the addressing at the originator processes
       MPI_Win_create(schedule, world_size * sizeof(long long), sizeof(long long), MPI_INFO_NULL,
          MPI_COMM_WORLD, &win);

       int ready = 0;
       while (!ready)
       {
          // Without the lock/unlock schedule stays forever filled with 0s
          MPI_Win_lock(MPI_LOCK_SHARED, 0, 0, win);
          ready = fnz(schedule, oldschedule, world_size);
          MPI_Win_unlock(0, win);
		  //printf("hello: %d\n",ready);
       }
	
		for(int i = 1 ; i<world_size ; i++){
			sum += schedule[i];
		}
       //printf("All workers checked in using RMA\n");

       // Release the window
       MPI_Win_free(&win);
       // Free the allocated memory
       MPI_Free_mem(schedule);
       free(oldschedule);

       //printf("Master done\n");
    }
	
    else
    {

       // Worker processes do not expose memory in the window
       MPI_Win_create(NULL, 0, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);

       // Register with the master
       MPI_Win_lock(MPI_LOCK_EXCLUSIVE, 0, 0, win);
       MPI_Put(&sum, 1, MPI_LONG_LONG, 0, world_rank, 1, MPI_LONG_LONG, win);
       MPI_Win_unlock(0, win);

       //printf("Worker %d finished RMA\n", world_rank);

       // Release the window
       MPI_Win_free(&win);

       //printf("Worker %d done\n", world_rank);
    }
    if (world_rank == 0)
    {

        // TODO: handle PI result
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
