#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main ()
{
	(void)umask(0);
	mknod("myfile.fifo", S_IFIFO|0666, 0);
	return 0;
}
