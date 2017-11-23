//Open file, add mem, write data from mid
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main (int argc, char* argv[])
{
	char data[] = "kek";
	struct stat buf;
	int fd;
	if (argc != 2)
	{
		perror("only 1 argument");
		exit(-1);
	}
	
	/*if (creat(argv[1], 0666)	 == -1)
	{
		perror("creat");
		exit(-1);
	}*/
	
	if ((fd = open(argv[1], 0666)) == -1)
	{
		perror("open");
		exit(-1);
	}
	
	if (stat(argv[1], &buf) == -1)
	{
		perror("stat");
		exit(-1);
	}
	
	if (ftruncate(fd, 2 * buf.st_size) == -1)
	{
		perror("stat");
		exit(-1);
	}
	
	if (lseek(fd, buf.st_size, SEEK_SET) == -1)
	{
		perror("stat");
		exit(-1);
	}
	
	if (write(fd, data, strlen(data)) == -1)
	{
		perror("stat");
		exit(-1);
	}
	
	return 0;
}
