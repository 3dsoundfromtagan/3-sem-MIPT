#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
	struct stat buf;
	if (argc != 2)
	{
		perror("only 1 argument");
		exit(-1);
	}
	
	if (stat(argv[1], &buf) == -1)
	{
		perror("stat");
		exit(-1);
	}
	printf ("dev = %d\nino = %d\nmode = %d\nlink = %d\nuid = %d\ngid = %d\nrdev = %d\nsize = %d\nblksize = %d\nblocks = %d\n", buf.st_dev, buf.st_ino, buf.st_mode, buf.st_nlink, buf.st_uid, buf.st_gid, buf.st_rdev, buf.st_size, buf.st_blksize, buf.st_blocks);
	return 0;
}
