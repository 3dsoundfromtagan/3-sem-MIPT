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
	char buf[BUF_SIZE]; 
	int f = open("in.txt", O_RDONLY|O_CREAT);
	int i = 0;
	ssize_t rr;
	if ((f == -1))
	{
		perror("open");
		exit(-1);
	}
	key_t k = ftok("./ex5_sender", 1);
	if (k == -1)
	{
		perror("ftok");
		exit(-2);
	}
	const size_t n = BUF_SIZE * sizeof(int);
	int s = shmget(k, n, 0666|IPC_CREAT);
	if (s == -1)
	{
		perror("shmget");
		exit(-3);
	}
	int* addr = shmat(s, NULL, 0);
	if (addr == (int*)(-1))
	{
		perror("shmat");
		exit(-4);
	}
	if ((rr = read(f, &buf, (ssize_t)BUF_SIZE)) == -1)							//After one compilation of three, a segmentation error appears
	{
		perror("read");
		exit(-3);
	}
	while (i < BUF_SIZE - 1)
	{
		*(addr + i) = buf[i];
		i++;
	}
	*(addr + i + 1) = '\n';
	
	if (close(f))
	{
		perror("Close");
		exit(-2);
	}
	return 0;
}
