/*Сервер 
 * Сообщает о подключении клиента всем клиентам
 * Принимает сообщение от клиента N и отправляет его либо всем, либо клиенту K
 * Тип сообщения - номер клиента?! Сервер принимает тип 0, клиенты - i
 * Задача 1. Заставить сервер писать сообщения о подключении
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include "protocol.h"

#define CMD_LEN 20

int msqid;

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

    while (1) {
        scanf("%s", command);
        if (strcmp(command, "write") == 0) {
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
            printf("%s\n", my_msg.info.text);

        } else if (strcmp(command, "quit") == 0) {
            my_msg.info.comm_type = DISCONNECT;
            send_message(&my_msg);
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
    return 0;
}
