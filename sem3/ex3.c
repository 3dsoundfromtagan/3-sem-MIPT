#include <stdio.h>
#include <unistd.h>
int main ()
{
	int a = fork();
	if (a > 0)
	{
		printf("I'm a parent. My PID is %d, child PID is %d\n", getpid(), a);
	}
	if (!a)
	{
		printf("I'm a child. Parent's PID is %d, my PID is %d\n", getppid(), getpid());
	}
	
	if (a < 0)
	{
		printf("Error\n");
	}
	
	return 0;
}
