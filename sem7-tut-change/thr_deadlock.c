#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

long i = 0;
sem_t sem1;
sem_t sem2;

void* inc(void* arg) {
	if(sem_wait(&sem1)) {
		perror("inc: sem1_wait");
		exit(-1);
	}
	sleep(2);
	if(sem_wait(&sem2)) {
		perror("inc: sem2_wait");
		exit(-2);
	}
	printf("in inc\n");
	i += 1000000;
	if(sem_post(&sem1)) {
		perror("inc: sem1_post");
		exit(-3);
	}
	if(sem_post(&sem1)) {
		perror("inc: sem1_post");
		exit(-4);
	}
}

void* dec(void* arg) {
	if(sem_wait(&sem2)) {
		perror("dec: sem2_wait");
		exit(-5);
	}
	sleep(2);
	if(sem_wait(&sem1)) {
		perror("dec: sem1_wait");
		exit(-6);
	}
	printf("in dec\n");
	i -= 1000000;
	if(sem_post(&sem2)) {
		perror("dec: sem2_post");
		exit(-7);
	}
	if(sem_post(&sem1)) {
		perror("dec: sem1_post");
		exit(-8);
	}
}

int main() {
	printf("in main\n");
	
	pthread_t thr_inc, thr_dec;

	if(sem_init(&sem1, 0, 1)) {
		perror("sem1_init");
		exit(-9);
	}
	
	if(sem_init(&sem2, 0, 1)) {
		perror("sem2_init");
		exit(-10);
	}
	
	if(pthread_create(&thr_inc, NULL, &inc, NULL)) {
		perror("Can't create thr_inc");
		exit(-11);
	}
	
	if(pthread_create(&thr_dec, NULL, &dec, NULL)) {
		perror("Can't create thr_dec");
		exit(-12);
	}
	
	if(pthread_join(thr_inc, NULL)) {
		perror("Can't join thr_inc");
		exit(-13);
	}
	
	if(pthread_join(thr_dec, NULL)) {
		perror("Can't join thr_dec");
		exit(-14);
	}
	
	printf("%ld\n", i);

	if(sem_destroy(&sem1)) {
		perror("sem1_destroy");
		exit(-15);
	}
	
	if(sem_destroy(&sem2)) {
		perror("sem2_destroy");
		exit(-16);
	}
	
	return 0;
}
