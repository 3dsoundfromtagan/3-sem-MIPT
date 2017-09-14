#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("Only 1 argument!\n");
		return -1;
	}
	
	int i = atoi(argv[1]);
	int pid = fork();
	while (i > 0)
	{
		if (!pid)
		{
			pid = fork();
			if (!pid)
			{
				printf("I'm a process. Parent's PID is %d, my PID is %d\n", getppid(), getpid());
			}
		}
		i--;
	}

	return 0;
}
