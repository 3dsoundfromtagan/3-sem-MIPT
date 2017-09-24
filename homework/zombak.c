#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main ()
{
	int a = fork();
	if (!a)
	{
		printf("%d %d", getpid(), getppid());
	}
	else 
	{
		sleep(1);
		system("ps -eo pid,ppid,stat,cmd");
		//wait();
	}
	
	return 0;
}
