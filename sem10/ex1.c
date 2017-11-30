#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define LEN 256

int main()
{
	char buf1[LEN];
	struct stat buf;
	scanf("%s", buf1);
	
	DIR* op = opendir(buf1);
	
	if (op == NULL)
	{
		perror("opendir");
		exit(-1);
	}
	
	struct dirent *dir;
	while ((dir = readdir(op)) != NULL)
	{
		if (stat(dir->d_name, &buf) == -1)
		{
			perror("stat");
			exit(-1);
		}
		printf("%s\n", dir->d_name);
		printf ("dev = %d\nino = %d\nmode = %d\nlink = %d\nuid = %d\ngid = %d\nrdev = %d\nsize = %d\nblksize = %d\nblocks = %d\n", buf.st_dev, buf.st_ino, buf.st_mode, buf.st_nlink, buf.st_uid, buf.st_gid, buf.st_rdev, buf.st_size, buf.st_blksize, buf.st_blocks);
		printf("**********************************************\n");
	}
	
	closedir(op);
	return 0;
}
