#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdlib.h>
 
int i = 1;
 
void* f(void* arg){
    int number = *(int*)arg;
    while(number != i){     
    }
    printf("%d\n", number);
    i++;
 
    return NULL;
}
 
 
int main(int argc, char *argv[]){
    if (argc < 2){
        printf("Должен быть минимум 1 параметр");
        exit(-1);
    }
    int n = atoi(argv[1]);
    printf("параметр: %d\n", n);
    pthread_t arr[n+1];
    
    int j;
    int data[n+1];
 
    for(j = 1; j <= n; j++){
        data[j] = j;
        if(pthread_create(&arr[j], NULL, &f, &data[j])){
            perror("pthread_create error");
            exit(-1);
        }
    }
 
    for(j = 1; j <= n; j++){
        if(pthread_join(arr[j], NULL)){
            perror("Can't join");
            exit(-1);
        }
    }
 
    return 0;
}
