#include <stdio.h>
#include <unistd.h>

int main ()
{
	printf("%d %d", getpid(), getppid());
	return 0;
}

