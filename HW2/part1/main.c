#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

pthread_t thread[1024];
pthread_mutex_t pi_mutex;


void *threadFunc(void *arg){
	long long toss = *(long long * )arg;
	//printf("im here %lu\n",pthread_self());
	//printf("toss:%lld\n",toss);
	double x = 0;
	double y = 0;
	long long sum = 0;
	for(int i =0 ; i<toss ; i++){
		x = (double)rand_r(&seed)/(RAND_MAX+1.0f);
		y= (double)rand_r(&seed)/(RAND_MAX+1.0f);
		if(x*x+y*y<1.0f)
			sum++;
	}
	pthread_mutex_lock(&pi_mutex);
	total_sum += sum;
	pthread_mutex_unlock(&pi_mutex);
	pthread_exit(NULL);
}

int main(int argc ,char **argv){
	int thread_num = 1;
	long long toss = 1000000; 
	long long toss_each = 0;

	clock_t time_begin, period;

	pthread_mutex_init(&pi_mutex, NULL);   	 
	
	if(argc==3){
		thread_num = atoi(argv[1]);
		toss = atoi(argv[2]);
	}


	toss_each = toss/thread_num;
	//thread = (pthread_t*)malloc(thread_num*sizeof(pthread_t));
	time_begin = clock(); 
	for(int i = 0;i<thread_num;i++){
		pthread_create(&thread[i],NULL,&threadFunc,(void *)&toss_each);
	}
	
	for(int i = 0;i<thread_num;i++){
		pthread_join(thread[i],NULL);
	}
	period = clock()-time_begin;
	pthread_mutex_destroy(&pi_mutex);
	printf("total_sum:%lld\n",total_sum);
	printf("PI:%0.8f\n",4.0f*(double)total_sum/(double)toss);
	printf ("wall clock time = %0.8f\n", (double)period/CLOCKS_PER_SEC);
	return 0;
}
