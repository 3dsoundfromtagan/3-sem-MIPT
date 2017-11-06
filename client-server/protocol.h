#include <errno.h>
#define MSGSZ 128
#define SERVER_TYPE 255


enum command_type {
    WRITE_ALL,
    WRITE,
    CONNECT,
    DISCONNECT
};

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

