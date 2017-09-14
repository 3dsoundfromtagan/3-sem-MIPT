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
	while (i > 0)
	{
		int a = fork();
		if (!a)
		{
			printf("I'm a process. Parent's PID is %d, my PID is %d\n", getppid(), getpid());
			i--;
		}
		i--;
	}
	

	
	/*if (a > 0)
	{
		printf("I'm a parent. My PID is %d, child PID is %d\n", getpid(), a);
	}
	if (!a)
	{
		printf("I'm a process. Parent's PID is %d, my PID is %d\n", getppid(), getpid());
	}
	
	if (a < 0)
	{
		printf("Error\n");
	}*/
	
	return 0;
}
