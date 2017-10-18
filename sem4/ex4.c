#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 64
/*FIFO Writer*/
int main (int argc, char* argv[])
{
	(void)umask(0);
	if (argc != 3)
	{
		perror("Only 3 arguments");
		exit(-1);
	}
	
	if (access(argv[1],F_OK))
	{
		//Check for existence
		int mkf = mknod(argv[1], S_IFIFO|0666, 0);
		if (mkf)
		{
			perror("Mknod");
			exit(-2);
		}
	}

	size_t f = open(argv[1], O_WRONLY|O_CREAT);
	
	if (f < 0)
	{
		perror("Open");
		exit(-2);
	}
	if (write(f, argv[2], BUF_SIZE) < 0)
	{
		perror("Write");
		exit(-3);
	}
	if (close(f))
	{
		perror("Close");
		exit(-4);
	}
	return 0;
}
