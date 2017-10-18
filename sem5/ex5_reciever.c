#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>

#define BUF_SIZE	64

int main ()
{
	int i = 0;
	char buf[BUF_SIZE];
	key_t k = ftok("./ex5_sender", 1);
	if (k == -1)
	{
		perror("ftok");
		exit(-2);
	}
	const size_t n = BUF_SIZE * sizeof(int);
	int s = shmget(k, n, 0666|IPC_CREAT);
	if (s < 0)
	{
		perror("shmget");
		exit(-3);
	}
	int* addr = (int*)shmat(s, NULL, 0);
	if (addr == (int*)(-1))
	{
		perror("shmat");
		exit(-4);
	}
	while(*(addr + i) != '\n')
	{
		buf[i] = *(addr + i);
		i++;
	}
	printf("%s", buf);
	
	if (shmdt(addr) == -1)
	{
		perror("shmdt");
		exit(-5);
	}
	if (shmctl(s, IPC_RMID, 0) == -1)
	{
		perror("shmctl");
		exit(-6);
	}
	return 0;
}
