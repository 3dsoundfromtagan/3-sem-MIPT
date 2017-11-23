//creates hard & soft link
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
	
	/*if ((fd = open(argv[1], 0666)) == -1)
	{
		perror("open");
		exit(-1);
	}*/
	
	if (link(argv[1], "2.txt") == -1)
	{
		perror("link");
		exit(-1);
	}
	
	if (symlink(argv[1], "3.txt") == -1)
	{
		perror("symlink");
		exit(-1);
	}
	
	
	
	return 0;
}
