//programm block while programm 3 don't work P()
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/ipc.h>


int main ()
{
	key_t k = ftok("./3_1", 1);
	const size_t n = 2* sizeof(int);
	int s = shmget(k, n, 0666|IPC_CREAT);
	int* addr = shmat(s, NULL, 0);
	*(addr) = 1;
	while (*(addr+sizeof(int))<*(addr))
		wait();
	*(addr+sizeof(int))=*(addr+sizeof(int))-1;
	printf("the first programm end");
	return 0;
}
