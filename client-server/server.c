/*Сервер
 * Сообщает о подключении клиента всем клиентам
 * Принимает сообщение от клиента N и отправляет его либо всем, либо клиенту K
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <zconf.h>
#include "protocol.h"

#define MAX_CLIENTS 20

pid_t client_pids[MAX_CLIENTS];
unsigned long clients_number = 0;
int msqid;

void add_client(pid_t pid) {
    client_pids[clients_number] = pid;
    clients_number++;
}

void remove_client(pid_t pid) {
    for (int i = 0; i < clients_number; i++) {
        if (client_pids[i] == pid) {
            client_pids[i] = client_pids[clients_number - 1];
            clients_number--;
        }
    }
}

void send_all(const char *const message) {
    msg my_msg;
    strcpy(my_msg.info.text, message);

    for (int i = 0; i < clients_number; i++) {
        my_msg.type = client_pids[i];
        printf("HUI Message:\ntype=%ld\ncomm_type=%d\npid=%d\ntext=\"%s\"\n\n", my_msg.type, my_msg.info.comm_type, my_msg.info.pid_from, my_msg.info.text);
        if (msgsnd(msqid, &my_msg, MSGSZ, 0) < 0) {
            perror("msgsnd");
            exit(-4);
        }
    }
}

void send_connect_message(pid_t pid) {
    char message[MSGSZ];
    sprintf(message, "Client with pid = %d connected", pid);
    send_all(message);
}


int main() {
    msg my_msg;
    key_t key;
    key = get_key_or_exit();
    msqid = get_msqid_or_exit(key);
    printf("Started server with msqid = %d, pid = %d, key = %d\n", msqid, getpid(), key);

    ssize_t ret;
    while ((ret = msgrcv(msqid, &my_msg, MSGSZ, SERVER_TYPE, 0)) != -1) {
        printf("Message:\ntype=%ld\ncomm_type=%d\npid=%d\ntext=\"%s\"\n\n", my_msg.type, my_msg.info.comm_type, my_msg.info.pid_from, my_msg.info.text);

        switch (my_msg.info.comm_type) {
            case WRITE_ALL:
                break;
            case WRITE:
                break;
            case CONNECT:
                add_client(my_msg.info.pid_from);
                send_connect_message(my_msg.info.pid_from);
                break;
            case DISCONNECT:
                remove_client(my_msg.info.pid_from);
                break;
        }
    }

    // failed to receive message, stop server
    perror("msgrcv");
    exit(-6);
}
