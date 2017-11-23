#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define LEN 256

int main()
{
	int fd;
	struct stat buf;
	size_t length;
	char path[LEN];
	int i;
	char *ptr, *tmpptr;
	scanf("%s", path);
	//scanf("%s", ptr);
	if ((fd = open(path, O_RDWR | O_CREAT, 0666)) == -1)
	{
		perror("open");
		exit(-1);
	}
	if (stat(path, &buf) == -1)
	{
		perror("stat");
		exit(-1);
	}
	ptr = (char*)mmap(NULL, buf.st_size + LEN, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
	{
		perror("mmap");
		exit(-1);
	}
	//tmpptr = ptr;
	*ptr = 'l';
	munmap((void*)tmpptr, 10);
	//tmpptr++;
	
	return 0;
}
