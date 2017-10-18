/*Массив из 3 чисел: число запусков 1, 2 прог и сумм обоих. При запуске 1 проги инкрем 1 и 2, при 2 - 2 и 3*/
#include <sys/types.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/ipc.h>


int main ()
{
	//const int mas[3] = {0, 0, 0};
	key_t k = ftok("./ex1", 1);
	const size_t n = 3 * sizeof(int);
	int s = shmget(k, n, 0666|IPC_CREAT);
	int* addr = shmat(s, NULL, 0);
	int* buf = addr;
	*(addr) = *(addr) + 1;
	addr = addr + 2 * sizeof(int);
	*addr = *addr + 1;
	printf("%d\n", s);
	printf("mas[0] = %d, mas[1] = %d, mas[2] = %d", *buf, *(buf + sizeof(int)), *(buf + 2 * sizeof(int)));
}
