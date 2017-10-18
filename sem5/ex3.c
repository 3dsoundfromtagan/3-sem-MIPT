/*Проверка работоспособности удаления*/
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/ipc.h>


int main ()
{
	key_t k = ftok("./ex1", 1);	
	const size_t n = 3 * sizeof(int);
	int s = shmget(k, n, 0666|IPC_CREAT);
	shmctl(s, IPC_RMID, NULL);
	shmdt(NULL);
}
