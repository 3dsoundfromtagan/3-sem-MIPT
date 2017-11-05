#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define BUF_SIZE	100

int main ()
{
	char buf_in[PATH_MAX];// = "abd";
	//int i = 0;
	int f = open("out.txt", O_WRONLY|O_CREAT, 0666);
	ssize_t ww;
	if(getwd(buf_in))
	{
		perror("getwd");
		exit(-1);
	}
	if (f == -1)
	{
		perror("Can't open files");
		exit(-1);
	}
	
	ww = write(f, &buf_in, (ssize_t)strlen(buf_in));
	if (ww < 0)
	{
		perror("Sheep");
		exit(-1);
	}

	close(f);
	return 0;
}
