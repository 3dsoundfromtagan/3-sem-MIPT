//Task 3. Semaphores based on SHM. There are 2 threads with critical section (inc and dec of a global variable i)
// and the variable addr delimits threads
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <stdlib.h>
int i = 0;
int* addr;
void* inc(void* arg)
{
	printf("in inc\n");
	//CRIT_SEC
	(*addr)--;
	if (*addr)
	{
		i += 100;
	}
	(*addr)++;
	//END_CRIT_SEC
	return NULL;
}

void* dec(void* arg)
{
	printf("in dec\n");
	//CRIT_SEC
	(*addr)--;
	if (*addr)
	{
		i -= 100;
	}
	(*addr)++;
	//END_CRIT_SEC
	return NULL;
}

int main ()
{
	pthread_t thr_inc, thr_dec;
	int n = 1 * sizeof(int);
	key_t k;
	if ((k = ftok("./task3", 1)) < 0)
	{
		perror("ftok");
		exit(-1);
	}
	int s; 
	if ((s = shmget(k, n, 0666|IPC_CREAT)) < 0)
	{
		perror("shmget");
		exit(-1);
	}
	if ((addr = shmat(s, NULL, 0)) < 0)
	{
		perror("shmat");
		exit(-1);
	}
	//now addr is our semaphore
	*addr = 1;
	
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
	if (shmdt(addr))
	{
		perror("shndt");
		exit(-2);
	}
	return 0;
}
