#include <stdio.h>
#include <unistd.h>
int main ()
{
	int a = fork();
	//fork();
	printf("Hello World! %d\n", a);
	return 0;
}
