#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int i = 0;
sem_t sem;

void* inc(void* arg)
{
	printf("in inc\n");
	//CRIT_SEC
	if(sem_wait(&sem))
	{
		perror("sem_wait(): inc");
		exit(-1);
	}
	i += 100;
	if(sem_post(&sem))
	{
		perror("sem_post(): inc");
		exit(-1);
	}
	//END_CRIT_SEC
	return NULL;
}

void* dec(void* arg)
{
	printf("in dec\n");
	//CRIT_SEC
	if(sem_wait(&sem))
	{
		perror("sem_wait(): dec");
		exit(-1);
	}
	i -= 100;
	if(sem_post(&sem))
	{
		perror("sem_post(): dec");
		exit(-1);
	}
	//END_CRIT_SEC
	return NULL;
}

int main()
{
	pthread_t thr_inc, thr_dec;
	
	if(sem_init(&sem, 0, 1))
	{
		perror("sem_init\n");
		exit(-1);
	}
	
	if (pthread_create(&thr_inc, NULL, inc, NULL))
	{
		perror("Cannot create inc-thread");
		exit(-1);
	}
	
	if (pthread_create(&thr_dec, NULL, dec, NULL))
	{
		perror("Cannot create dec-thread");
		exit(-2);
	}
	
	//Ожидание соотв. потока
	if(pthread_join(thr_inc, NULL))
	{
		perror("Can't join thr_inc");
		exit(-3);
	}
	
	if(pthread_join(thr_dec, NULL))
	{
		perror("Can't join thr_dec");
		exit(-4);
	}
	
	printf("i = %d", i);
	
	if(sem_destroy(&sem))
	{
		perror("sem_destroy\n");
		exit(-1);
	}
}
