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

#define BUF_SIZE	6

int main ()
{
	//char buf[BUF_SIZE];
	int i = 0;
	key_t k = ftok("./ex5_sender", 1);
	const size_t n = BUF_SIZE * sizeof(int);
	int s = shmget(k, n, 0666|IPC_CREAT);
	int* addr = shmat(s, NULL, 0);
	
	for(i = 0; i < BUF_SIZE; i++)
	{
		printf("*addr + %d =  %c\n", i, *(addr + sizeof(char) * i));
	}
	
	/*
	while(i < 4)
	{
		buf[i] = *(addr + i);
		i++;
	}
	printf("%s", buf);*/
	return 0;
}
