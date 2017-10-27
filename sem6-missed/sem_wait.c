#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main ()
{
	int semid;
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
