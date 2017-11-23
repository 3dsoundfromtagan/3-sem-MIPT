/*Клиент
 * Принимает, отправляет или отправляет всем сообщения
*/
<<<<<<< HEAD
#include "protocol.h"
=======
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
>>>>>>> ca241d01634345094bcb0e67bb42d417a88f0b07

#define CMD_LEN 20

int msqid;

<<<<<<< HEAD
void send_message(msg *const message) {
    message->type = SERVER_TYPE;
    message->info.pid_from = getpid();
    if (msgsnd(msqid, message, MSGSZ, 0) < 0) {
        perror("msgsnd");
        exit(-4);
    }
}


int main() {
    char command[CMD_LEN];
    key_t key;
    msg my_msg;
    pid_t pid = getpid();
    key = get_key_or_exit();
    msqid = get_msqid_or_exit(key);
    printf("Started client with pid = %d, msqid = %d, key = %d\n", pid, msqid, key);

    my_msg.info.comm_type = CONNECT;
    send_message(&my_msg);
=======
typedef struct _msg {
    long type;
    struct {
        pid_t pid;
        char text[MSGSZ];
    } info;
} msg;

int main() {
    char command[CMD_LEN];
    int msqid;
    key_t key;
    size_t msg_length;
    pid_t id = getpid();
    //int i = 0;

    if ((key = ftok("./server", 0)) < 0) {
        perror("ftok");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) < 0) {
        if (errno != EEXIST) {
            perror("Can't creat msgq");
            exit(-2);
        } else {
            if ((msqid = msgget(key, 0)) < 0) {
                perror("Can't find msgq");
                exit(-3);
            }
        }
    }

    msg *my_msg = (msg *) malloc(sizeof(msg));

    (my_msg)->type = 255;
    strcpy((my_msg)->info.text, "Connected");
    (my_msg)->info.pid = id;
    //msg_length = strlen((my_msg)->info.text) + 1;
    if (msgsnd(msqid, (my_msg), MSGSZ, 0) < 0) {
        perror("msgsnd");
        exit(-4);
    }
>>>>>>> ca241d01634345094bcb0e67bb42d417a88f0b07

    while (1) {
        scanf("%s", command);
        if (strcmp(command, "write") == 0) {
<<<<<<< HEAD
            scanf("%d", &(my_msg.info.pid_to));
            scanf("%s", my_msg.info.text);
            my_msg.info.comm_type = WRITE;
            send_message(&my_msg);

        } else if (strcmp(command, "wrall") == 0) {
            scanf("%s", my_msg.info.text);
            my_msg.info.comm_type = WRITE_ALL;
            send_message(&my_msg);

        } else if (strcmp(command, "read") == 0) {
            if (msgrcv(msqid, &my_msg, MSGSZ, pid, 0) < 0) {
                perror("msgrcv");
                exit(-6);
            }
            if ((my_msg.info.comm_type == WRITE) || (my_msg.info.comm_type == WRITE_ALL)) {
				printf("%d: %s\n", my_msg.info.pid_from, my_msg.info.text);
			}
			if ((my_msg.info.comm_type == CONNECT) || (my_msg.info.comm_type == DISCONNECT)) {
				printf("%s\n", my_msg.info.text);
			}

        } else if (strcmp(command, "quit") == 0) {
            my_msg.info.comm_type = DISCONNECT;
            send_message(&my_msg);
            exit(0);
        }
    }
=======

        }

        if (strcmp(command, "wrall") == 0) {

        }

        if (strcmp(command, "read") == 0) {
            //shit is here
            if (msgrcv(msqid, my_msg, MSGSZ, 0, 0) < 0) {
                perror("msgrcv");
                exit(-6);
            }
            printf("%s\n", my_msg->info.text);
        }
        if (strcmp(command, "quit") == 0) {
            (my_msg)->type = SERVER_TYPE;
            strcpy((my_msg)->info.text, "Disonnected");
            //msg_length = strlen((my_msg)->text) + 1;
            if (msgsnd(msqid, (my_msg), MSGSZ, 0) < 0) {
                perror("msgsnd");
                exit(-4);
            }
            exit(0);
        }
    }





    // code for SENDER
    //msg* my_msg = (msg*)malloc(2 * sizeof(msg));

    /*(my_msg + 1)->type = LAST_MESSAGE;
    msg_length = 0;
    if (msgsnd(msqid, (my_msg + 1), msg_length, 0) < 0) {
        perror("msgsnd");
        exit(-5);
    }*/
    free(my_msg);
>>>>>>> ca241d01634345094bcb0e67bb42d417a88f0b07
    return 0;
}
