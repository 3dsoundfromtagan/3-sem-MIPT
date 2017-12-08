#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/shm.h>

#define MSGSZ 128
#define SERVER_TYPE LONG_MAX - 15
#define MAX_CLIENTS 20

enum command_type {
    WRITE_ALL,
    WRITE,
    CONNECT,
    DISCONNECT
};

typedef struct _msg_shm {
    pid_t pid;
    int counter;
} msg_shm;

typedef struct _msg {
    long type;
    struct {
        pid_t pid_from;
        pid_t pid_to;
        enum command_type comm_type;
        char text[MSGSZ];
    } info;
} msg;


key_t get_key_or_exit() {
    key_t key;
    if ((key = ftok("./server", 0)) < 0) {
        perror("ftok");
        exit(-1);
    }
    return key;
}

int get_msqid_or_exit(key_t key) {
    int _msqid;
    if ((_msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            perror("Can't creat msgq");
            exit(-2);
        } else {
            if ((_msqid = msgget(key, 0)) < 0) {
                perror("Can't find msgq");
                exit(-3);
            }
        }
    }
    return _msqid;
}

void get_shm(key_t key, msg_shm* message_counter) {
    int _shmid;
    if ((_shmid = shmget(key, MAX_CLIENTS * sizeof(msg_shm), 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            perror("Can't creat msgq");
            exit(-2);
        } else {
            if ((_shmid = shmget(key, MAX_CLIENTS * sizeof(msg_shm), 0)) < 0) {
                perror("Can't find msgq");
                exit(-3);
            }
        }
    }
    
    if ((message_counter = (msg_shm *)shmat(_shmid, NULL, 0)) == (msg_shm *)(-1)) {
		perror("shmat");
		exit(-4);
	}
}

void detach_shm (msg_shm* message_counter) {
	if (shmdt(message_counter) < 0) {
		perror("shmdt");
		exit(-5);
	}
}
