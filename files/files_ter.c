#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h >
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>

#define BUF_SIZE	6

int main ()
{
	char buf_in[PATH_MAX] = "mamahohotala";  
	//char buf_out[BUF_SIZE];  
	int i = 0;
	int fi = open("in.txt", O_RDONLY);
	int fo = open("out.txt", O_WRONLY);
	ssize_t rr, ww;
	if ((fi == -1) || (fo == -1))
	{
		perror("Can't open files");
		exit(-1);
	}
	
	while ((rr = read(fi, &buf_in, (ssize_t)BUF_SIZE)) > 0)
	{
		/*for (i = 0; i < sizeof(buf_in); i++)
		{
			buf_in[i] = buf_in[i]//toupper(buf_in[i]);
		}*/
	
		ww = write(fo, &buf_in, (ssize_t)BUF_SIZE);
		if (ww != rr)
		{
			perror("Sheep");
			exit(-1);
		}
	}
	
	close(fo);
	close(fi);
	return 0;
}
