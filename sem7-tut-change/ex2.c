/* Создать N потоков, каждый из них получает свой номер [0..N)
 * Потоки пишут на экран свои номера
 * Синхронизировать потоки семафорами так, чтобы числа шли по порядку */
 //Костыль, который непонятно почему работает. Омерзительное решение.
 //Не менее мерзкий костыль почему-то немного ускоряет время работы программы
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int i = 0;

void* num(void* args)
{
	int *num = (int*)args;
	while (*num != i)
	{
		usleep (10);
	}
	printf("%d\n", *(num));
	i++;
	return NULL;
}

int main ()
{
	int n = 0;
	int j = 0;
	scanf("%d", &n);
	pthread_t* thr_mas = (pthread_t*)(malloc(n * sizeof(pthread_t)));
	int* data = (int*)malloc(n * sizeof(int));
	
	for (j = 0; j < n; j++)
	{
		data[j] = j;
		if (pthread_create(&thr_mas[j], NULL, num, (void*)(data + j)))
		{
			perror("Cannot create thread");
			exit(-1);
		}
	}
	
	for (j = 0; j < n; j++)
	{
		if (pthread_join(thr_mas[j], NULL))
		{
			perror("Cannot join thread");
			exit(-2);
		}
	}
	
	free(thr_mas);
	free(data);
	return 0;
}
 
