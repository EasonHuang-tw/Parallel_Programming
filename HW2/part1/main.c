#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <immintrin.h>

pthread_t thread[1024];
pthread_mutex_t pi_mutex;
long long total_sum = 0;

float sum8(__m256 x) {
    // hiQuad = ( x7, x6, x5, x4 )
    const __m128 hiQuad = _mm256_extractf128_ps(x, 1);
    // loQuad = ( x3, x2, x1, x0 )
    const __m128 loQuad = _mm256_castps256_ps128(x);
    // sumQuad = ( x3 + x7, x2 + x6, x1 + x5, x0 + x4 )
    const __m128 sumQuad = _mm_add_ps(loQuad, hiQuad);
    // loDual = ( -, -, x1 + x5, x0 + x4 )
    const __m128 loDual = sumQuad;
    // hiDual = ( -, -, x3 + x7, x2 + x6 )
    const __m128 hiDual = _mm_movehl_ps(sumQuad, sumQuad);
    // sumDual = ( -, -, x1 + x3 + x5 + x7, x0 + x2 + x4 + x6 )
    const __m128 sumDual = _mm_add_ps(loDual, hiDual);
    // lo = ( -, -, -, x0 + x2 + x4 + x6 )
    const __m128 lo = sumDual;
    // hi = ( -, -, -, x1 + x3 + x5 + x7 )
    const __m128 hi = _mm_shuffle_ps(sumDual, sumDual, 0x1);
    // sum = ( -, -, -, x0 + x1 + x2 + x3 + x4 + x5 + x6 + x7 )
    const __m128 sum = _mm_add_ss(lo, hi);
    return _mm_cvtss_f32(sum);
}

void *threadFunc(void *arg){
	unsigned int seed = 1;
	long long toss = *(long long *)arg;
	long long sum = 0;
	avx_xorshift128plus_key_t mykey;	
	avx_xorshift128plus_init(324,4444,&mykey);
	__m256 one = _mm256_set1_ps(1.0f);
	__m256 zero = _mm256_setzero_ps();
	__m256 v_INT32_MAX = _mm256_set1_ps(INT32_MAX);
	__m256 v_x , v_y , v_x2_y2_plus , mask ,v_result;
	
	for(int i = 0;i<toss;i+=8){
		__m256i rand_i = avx_xorshift128plus(&mykey);
		v_x = _mm256_cvtepi32_ps(rand_i);
		v_x = _mm256_div_ps(v_x,v_INT32_MAX)
		
		__m256i rand_i = avx_xorshift128plus(&mykey);
		v_x = _mm256_cvtepi32_ps(rand_i);
		v_x = _mm256_div_ps(v_x,v_INT32_MAX);

		v_x2_y2_plus = _mm256_add_ps(_mm256_mul_ps(v_x,v_x),_mm256_mul_ps(v_y,v_y));
		mask = _mm256_cmp_ps(one,v_x2_y2_plus,13);
		v_result = _mm256_and_ps(mask,one);
		sum += (long long)sum8(v_result); 
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

	pthread_mutex_init(&pi_mutex, NULL);   	 
	
	if(argc==3){
		thread_num = atoi(argv[1]);
		toss = atoi(argv[2]);
	}


	toss_each = toss/thread_num;
	//thread = (pthread_t*)malloc(thread_num*sizeof(pthread_t));
	for(int i = 0;i<thread_num;i++){
		pthread_create(&thread[i],NULL,&threadFunc,(void *)&toss_each);
	}
	
	for(int i = 0;i<thread_num;i++){
		pthread_join(thread[i],NULL);
	}
	pthread_mutex_destroy(&pi_mutex);
	//printf("total_sum:%lld\n",total_sum);
	printf("%0.8f",4.0f*(double)total_sum/(double)toss);
	//printf ("wall clock time = %0.8f\n", (double)period/CLOCKS_PER_SEC);
	return 0;
}
