#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>

#define BUF_SIZE 128
#define SEM_NAME_1 "/sem1"
#define SEM_NAME_2 "/sem2"
sem_t *sem1, *sem2;
 
int main(int argc, char* argv[]) {
   
    if (argc != 3) {
        perror("arguments");
        exit(-1);
    }
   
    int fd[2];
    char buffer_parent[BUF_SIZE];
	char buffer_child[BUF_SIZE];
   
    if (pipe(fd) < 0) {
        perror("pipe");
        exit (-1);
    }
 
    int a = fork();
 
    if (a < 0) {
        perror("fork");
        exit (-1);
    }
 
    if (a > 0) {
        //PARENT	
		if( (sem1 = sem_open(SEM_NAME_1, O_CREAT, 0777, 1)) == SEM_FAILED) {
			perror("parent sem1_open");
			exit (-1);
		}
		if( (sem2 = sem_open(SEM_NAME_2, O_CREAT, 0777, 0)) == SEM_FAILED) {
			perror("parent: sem2_open");
			exit (-1);
		}
		
		//CRIT SEC
		if (sem_wait(sem1)) {
			perror ("parent: sem1_wait");
			exit (-1);
		}
		if (write(fd[1], argv[1], BUF_SIZE) < 0) {
            perror("parent: write");
            exit(-1);
        }
		if (sem_post(sem2)) {
			perror ("parent: sem2_post");
			exit (-1);
		}
		//END OF CRIT SEC
		
		if(close(fd[1])<0) {
            perror("close");
            exit(-5);
        }
		
		//CRIT SEC
        if (sem_wait(sem1)) {
			perror ("parent: sem1_wait");
			exit (-1);
		}
		if (read(fd[0], buffer_parent, BUF_SIZE) < 0) {
			perror("read");
			exit(-1);
		}
		//END OF CRIT SEC
		
		if (sem_post(sem1)) {
			perror ("parent: sem1_post");
			exit (-1);
		}
		
		if(close(fd[0])<0) {
            perror("close");
            exit(-1);  
        }
		
		printf("parent sends: %s\n", argv[1]);
		printf("parent receives: %s\n", buffer_parent);
	}
       
	else if (!a)
	{
		//CHILD

		if( (sem1 = sem_open (SEM_NAME_1, O_CREAT, 0777,1)) == SEM_FAILED) {
			perror("parent: sem1_open");
			exit (-1);
		}
		if( (sem2 = sem_open (SEM_NAME_2, O_CREAT, 0777,0)) == SEM_FAILED) {
			perror("parent: sem2_open");
			exit (-1);
		}		
        
		//CRIT SEC
		if (sem_wait(sem2)) {
			perror ("child: sem1_wait");
			exit (-1);
		}
		if (read(fd[0], buffer_child, BUF_SIZE) < 0) {
			perror("read");
			exit(-1);
		}
		if (sem_post(sem2)) {
			perror ("child: sem2_post");
			exit (-1);
		}
		//END OF CRIT SEC
		
		if(close(fd[0]) < 0) {
			perror("close");
			exit(-1);
		}
		
		//CRIT SEC
		if (write(fd[1], argv[2], BUF_SIZE) < 0) {
            perror("child: write");
            exit(-1);
        }
		if (sem_post(sem1)) {
			perror ("child: sem2_post");
			exit (-1);
		}
		//END OF CRIT SEC
		
		if (sem_wait(sem2)) {
			perror ("child: sem2_wait");
			exit (-1);
		}
	
		if(close(fd[1]) < 0) {
			perror("close");
			exit(-1);
		}
		
		printf("child sends: %s\n", argv[2]);
		printf("child receives: %s\n", buffer_child);
    }

	if (sem_close(sem1)) {
		perror("sem1_close");
		exit(-1);
	}	
	if (sem_close(sem2)) {
		perror("sem2_close");
		exit(-1);
	}
	
	return 0; 
}
