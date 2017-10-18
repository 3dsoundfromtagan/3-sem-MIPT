#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 64
/*FIFO Reader*/
int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		perror("Only 2 arguments");
		exit(-1);
	}
	(void)umask(0);
	char mas[BUF_SIZE];
	
	if (access(argv[1], F_OK))
	{
		//Check for existence
		int mkf = mknod(argv[1], S_IFIFO|0666, 0);
		if (mkf)
		{
			perror("Mknod");
			exit(-2);
		}
	}
	
	size_t f = open(argv[1], O_RDONLY|O_CREAT);
	
	if (f < 0)
	{
		perror("open");
		exit(-2);
	}
	
	if (read(f, mas, BUF_SIZE) < 0)
	{
		perror("read");
		exit(-3);
	}
	if (close(f))
	{
		perror("close");
		exit(-4);
	}
	printf("%s", mas);
	return 0;
}
