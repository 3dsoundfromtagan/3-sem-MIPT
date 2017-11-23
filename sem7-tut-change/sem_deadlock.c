#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>

#define SEM_NAME_1 "/sem1"
#define SEM_NAME_2 "/sem2"

sem_t *sem1, *sem2;

int main ()
{
	int pid = fork();
	
	if (pid < 0) {
		perror("fork");
		exit (-1);
	}

	if (pid > 0) {
		if( (sem1 = sem_open (SEM_NAME_1, O_CREAT, 0777,1)) == SEM_FAILED) {
			perror("parent: sem_open");
			exit (-2);
		}
		if( (sem2 = sem_open (SEM_NAME_2, O_CREAT, 0777,1)) == SEM_FAILED) {
			perror("parent: sem_open");
			exit (-3);
		}

		int i = 0;
		//CRIT SEC
		//p(s)	
		if (sem_wait(sem1)) {
			perror ("parent: sem_open");
			exit (-4);
		}
		sleep(5);	
		if (sem_wait(sem2)) {
			perror ("parent: sem_open");
			exit (-5);
		}
		for (i = 0; i<100; i++) { 
			printf("parent\n");
		}
		if (sem_post(sem1)) {
			perror ("parent: sem_post");
			exit (-6);
		}
		if (sem_post(sem2)) {
			perror ("parent: sem_post");
			exit (-7);
		}
		//v(s)
		//END OF CRIT SEC
	}
	
	if (!pid) {
		if( (sem2 = sem_open (SEM_NAME_2, O_CREAT, 0777,1)) == SEM_FAILED) {
			perror("child: sem_open");
			exit (-8);
		}
		if( (sem1 = sem_open (SEM_NAME_1, O_CREAT, 0777,1)) == SEM_FAILED) {
			perror("child: sem_open");
			exit (-9);
		}

		int i = 0;
		//CRIT SEC
		//p(s)	
		if (sem_wait(sem2)) {
			perror ("child: sem_open");
			exit (-10);
		}	
		if (sem_wait(sem1)) {
			perror ("child: sem_open");
			exit (-11);
		}
		for (i = 0; i<100; i++)	{
			printf("child\n");
		}
		if (sem_post(sem2)) {
			perror ("child: sem_post");
			exit (-12);
		}
		if (sem_post(sem1)) {
			perror ("child: sem_post");
			exit (-13);
		}
		//v(s)
		//END OF CRIT SEC
	}

	if (sem_close(sem1)) {
		perror("sem_close");
		exit(-14);
	}
	if (sem_close(sem2)) {
		perror("sem_close");
		exit(-15);
	}	
	
	return 0;
}