#include	<stdio.h>
#include	<pthread.h>
#define	SIZE 10
#define ITERATIONS 100000

void *myfn(void *ptr);

int global_index = 0;
pthread_mutex_t m;

int main()
{
	pthread_t thread[SIZE];
	int i;
	
	for(i = 0; i < SIZE; i++)
		pthread_create(&thread[i], NULL, myfn, NULL);
	
	for(i = 0; i < SIZE; i++)
		pthread_join(thread[i], NULL);
	
	printf("final value of global_Index = %d\n", global_index);
	
	return 0;
}

void *myfn(void *ptr)
{
	int i;
	
	pthread_mutex_lock(&m); //😀
	for(i = 0; i < ITERATIONS; i++)
		global_index++;
	pthread_mutex_unlock(&m); //😀
	printf("after this iteration global_index = %d\n", global_index);
}
