//another task3 with 2 process. This process wait while 'task3_shm' start.
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>


int main ()
{
	key_t k;
	if ((k = ftok("./task3_1_shm" ,1)) < 0)
	{
		perror("ftok");
		exit(-1);
	}
	const size_t n = 3 * sizeof(int);
	int s;
	if ((s = shmget(k, n, 0666|IPC_CREAT)) < 0)
	{
		perror("shmget");
		exit(-2);
	}
	int* addr;
	if ((addr = shmat(s, NULL, 0)) < 0)
	{
		perror("shmat");
		exit(-3);
	}	
	*(addr) = 1;
	while (*(addr+sizeof(int)) < *(addr))
	{
		wait();
	}
	
	*(addr+sizeof(int))=*(addr+sizeof(int))-1;
	printf("The first programme ends");
	if (shmdt(addr))
	{
		perror("shmdt");
		exit(-4);
	}
	return 0;
}
