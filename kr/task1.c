//Task 1. Creates n proccesses, childs send "signal" to parent. Parent recieves it and prints the number of them.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//Based on ex3 sem3

#define BUF_SIZE 10

int main (int argc, char* argv[])
{
	int fd[2];
	//message to be send
	char mas[] = "signal";
	//bufer to be recieve
	char buf1[BUF_SIZE] = "000";
	int count = 1;
	
	if ((pipe(fd) < 0))
	{
		perror("pipe");
		exit(-1);
	}
	
	if (argc != 2)
	{
		printf("Only 1 argument\n");
		exit(-1);
	}
	
	int i = atoi(argv[1]);
	
	if (i <= 0)
	{
		printf("Number must be natural\n");
		exit(-1);
	}
	
	int pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(-1);
	}
	while (i > 0)
	{
		if (!pid)
		{
			//child
			pid = fork();
			if (!pid)
			{
				if (write(fd[1], mas, strlen(mas)) < 0)
				{
					perror("write");
					exit(-4);
				}
				//writing was successful
				count++;
			}
			else
			{
				//parent
				if (read(fd[0], buf1, BUF_SIZE) < 0)
				{
					perror("read");
					exit(-5);
				}
				printf("%d signals recieved from childs\n", count);
			}
		}
		i--;
	}
	close(fd[0]);
	close(fd[1]);
	return 0;
}
