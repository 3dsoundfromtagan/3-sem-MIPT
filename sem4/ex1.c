#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUF_SIZE 3

int main()
{
	char mas[] = "123";
	char buf[BUF_SIZE];
	int pipefd[2];
	pipe(pipefd);
	
	write(pipefd[1], mas, BUF_SIZE);
	read(pipefd[0], buf, BUF_SIZE);
	
	printf("%s", buf);
	
	return 0;
}
