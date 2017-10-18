#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 128

int main(int argc, char* argv[]) 
{
	
	if (argc != 2)
	{
		perror("arguments\n");
		exit(-1);
	}
	
	int fd[2];
	ssize_t ww, rr;
	char buffer[BUF_SIZE];
	
	if (pipe(fd) < 0)
	{
		perror("pipe\n");
		exit (-2);
	}

	int a = fork();

	if (a < 0)
	{
		perror("fork");
		exit (-2);
	}

	if (a > 0) {
		//PARENT
		
		printf("%d %d\n", getpid(), getppid());
		
		if(close(fd[0])<0) 
		{
			perror("close");
			exit(-3);	
		}
	 
		ww = write(fd[1], argv[1], BUF_SIZE);
		if (ww < 0) 
		{
			perror("write");
			exit(-4);
		}
		printf("%d\n", ww);
		
		if(close(fd[1])<0) 
		{
			perror("close");
			exit(-5);
		}
		}
		
		else if (!a) 
		{
			//CHILD
			
			if(close(fd[1])<0) 
			{
				perror("close");
				exit(-6);
			}
			rr = read(fd[0], buffer, BUF_SIZE);
			if (rr < 0) {
				perror("read");
				exit(-7);
			}

			if(rr == ww)
			{
				perror("read error");
				exit(-8);
			}

			printf("%s %d %d\n", buffer, getpid(), getppid());
			
			if(close(fd[0])<0) 
			{
				perror("close");
				exit(-9);
			}
	}

return(0);	

}
