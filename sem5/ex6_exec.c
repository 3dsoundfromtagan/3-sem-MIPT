#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 128

int main()
{
	int i = 0;															//Counter
	int n = 0;															//Num of arguments
	char path[BUF_SIZE];
	
	while(1)
	{
		printf("Enter path or 'quit': ");
		scanf("%s", path);
		if (!strcmp(path, "quit"))
		{
			break;
		}
		printf("Enter number of arguments: ");
		scanf("%d", &n);
		char** mas = (char**)malloc(n * sizeof(char*));						//Arguments (array of strings)
		for (i = 0 ; i < n; i++)
		{
			mas[i] = (char*)malloc(BUF_SIZE * sizeof(char));
			scanf("%s", mas[i]);
		}
		mas[n] = (char*)NULL;											//The array of pointers must be terminated by a null pointer
		
		int a = fork();
		
		if (a == -1)
		{
			perror("fork");
			exit(-1);
		}
		
		if (!a)
		{
			//Child
			if (execv(path, mas) == -1)
			{
				perror("execv");
				exit(-2);
			}
			for (i = 0; i < n; i++)
			{
				free(mas[i]);
			}
			free(mas);
		}
		if (a > 0)
		{
			//Parent is waiting for the child not to become a zombie
			wait(NULL);
		}
	}
	return 0;
}
