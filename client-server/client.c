/*Сервер 
 * Сообщает о подключении клиента всем клиентам
 * Принимает сообщение от клиента N и отправляет его либо всем, либо клиенту K
 * Тип сообщения - номер клиента?! Сервер принимает тип 0, клиенты - i
 * Задача 1. Заставить сервер писать сообщения о подключении
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>

#define LAST_MESSAGE 200
#define MSGSZ 128
#define CMD_LEN 20

#define WRALL_TYPE 254
#define SERVER_TYPE 255
#define QUIT_TYPE 256

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

    while (1) {
        scanf("%s", command);
        if (strcmp(command, "write") == 0) {

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
    return 0;
}
