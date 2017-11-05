#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
	int semid;
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
	
	key_t k;
	struct sembuf s;
	
	if ((k = ftok("./sem_wait", 0)) < 0)
	{
		perror("ftok");
		exit(-1);
	}
	
	if ((semid = semget(k, 1, 0666 | IPC_CREAT)) < 0)
	{
		perror("semid");
		exit(-2);
	}
	
	s.sem_num = 0;
	s.sem_op = -5;
	s.sem_flg = 0;
	
	if (semop(semid, &s, 1) < 0)
	{
		perror("semop");
		exit(-3);
	}
	
	printf("Finished\n");
	
	return 0;
}
