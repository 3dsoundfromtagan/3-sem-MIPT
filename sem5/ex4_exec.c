#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <sys/wait.h>

#define BUF_SIZE 128

int main() {
	
	char path[BUF_SIZE];
	char** arg;
	int N = 0;
	int i = 0;
	
	while (1) {
		printf("I need path: ");
		scanf("%s", path);		
		if (!strcmp(path, "quit")) {
			break;
		}
		printf("I need number of arguments: ");
		scanf("%d", &N);
		arg = (char**) malloc ((N)*sizeof(char*));
		printf("I need arguments: ");
		for (i = 0; i < N; i++) {
			arg[i] = (char*) malloc (BUF_SIZE*sizeof(char));
			scanf("%s", arg[i]);
		}
		arg[N] = (char*)NULL;

		i = 0;
		
		int a = fork();
	
		if (a < 0) {
			perror("fork");
			exit (-1);
		}
	
		if (!a) {
			//CHILD

			if (execv(path, arg) == -1) {
					perror("execv");
					exit(-2);
			}
			
			for (i = 0; i < N; i++) {
				free(arg[i]);
			}
			free(arg);
		}
		else {
			wait();
		}

		i = 0;
	}
	return 0;
}
