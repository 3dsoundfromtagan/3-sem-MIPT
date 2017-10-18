#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 3

int main()
{
	//Message to be send from parent to child
	char mas1[] = "123";
	//Message to be send from child to parent
	char mas2[] = "456";
	//Message to be recieve from parent by child
	char buf1[BUF_SIZE] = "000";
	//Message to be recieve from child by parent
	char buf2[BUF_SIZE] = "000";

	int fd1[2];
	int fd2[2];
	if ((pipe(fd1) < 0))
	{
		perror("Pipe 1\n");
		exit(-1);
	}
	
	if ((pipe(fd2) < 0))
	{
		perror("Pipe 2\n");
		exit(-1);
	}
	
	int a = fork();
	
	if (a == -1)
	{
		perror("Fork\n");
		exit(-2);
	}

	if (a == 0)
	{
		//Child
		if (close(fd1[0]) < 0)
		{
			perror("Close 1[0]");
			exit(-3);
		}
		if (write(fd1[1], mas2, strlen(mas2)) < 0)
		{
			perror("Write 1");
			exit(-4);
		}
		if (read(fd2[0], buf1, BUF_SIZE) < 0)
		{
			perror("Read 2");
			exit(-5);
		}
		if (close(fd1[1]) < 0)
		{
			perror("Close 1[1]");
			exit(-3);
		}
		
	}
	
	if (a > 0)
	{
		//Parent
		if (close(fd2[0]) < 0)
		{
			perror("Close 2[0]");
			exit(-3);
		}
		if (write(fd2[1], mas1, strlen(mas1)) < 0)
		{
			perror("Write 2");
			exit(-4);
		}
		if (read(fd1[0], buf2, BUF_SIZE) < 0)
		{
			perror("Read 1");
			exit(-5);
		}
		if (close(fd2[1]) < 0)
		{
			perror("Close 1[1]");
			exit(-3);
		}
	}
	
	printf("P -> C: %s	C -> P: %s	C <- P: %s	P <- C: %s	proc_ind: %d\n", mas1, mas2, buf1, buf2, a);
	return 0;
}
