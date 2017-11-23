/* Создать N потоков, каждый из них получает свой номер [0..N)
 * Потоки пишут на экран свои номера
 * Синхронизировать потоки семафорами так, чтобы числа шли по порядку */
 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int i = 0;
sem_t sem; 

void* num(void* args)
{
	//CRIT_SEC
	if(sem_wait(&sem))
	{
		perror("sem_wait()");
		exit(-1);
	}

	printf("%d\n", ((int*)args));
	if(sem_post(&sem))
	{
		perror("sem_post()c");
		exit(-1);
	}
	//END_CRIT_SEC*/
	return 0;
}

int main ()
{
	int n = 0;
	scanf("%d", &n);
	pthread_t* thr_mas = (pthread_t*)(malloc(n * sizeof(pthread_t)));
	
	if(sem_init(&sem, 0, 1))
	{
		perror("sem_init\n");
		exit(-1);
	}
	
	for (i = 0; i < n; i++)
	{
		if (pthread_create(&thr_mas[i], NULL, num, (void*)(i)))
		{
			perror("Cannot create thread");
			exit(-1);
		}
		if (pthread_join(thr_mas[i], NULL))
		{
			perror("Cannot join thread");
			exit(-2);
		}
	}
	
/*	for (i = 0; i < n; i++)
	{
		if (pthread_join(thr_mas[i], NULL))
		{
			perror("Cannot join thread");
			exit(-2);
		}
	}
*/	
	if(sem_destroy(&sem))
	{
		perror("sem_destroy\n");
		exit(-1);
	}
	
	free(thr_mas);
	return 0;
}
