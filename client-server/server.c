/*Сервер 
 * Сообщает о подключении клиента всем клиентам
 * Принимает сообщение от клиента N и отправляет его либо всем, либо клиенту K
 * Тип сообщения - номер клиента?! Сервер принимает тип 255, клиенты - i, посылка всем - 254
 * Задача 1. Заставить сервер писать сообщения о 
 * КАК НУМЕРОВАТЬ КЛИЕНТОВ? Client N (dis)connected???
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MSGSZ 128
#define WRALL_TYPE 254
#define SERVER_TYPE 255
#define QUIT_TYPE 256
#define MAX_CLIENTS 20


typedef struct _msg {
    long type;
    struct {
        pid_t pid;
        char text[MSGSZ];
    } info;
} msg;

pid_t client_pids[MAX_CLIENTS];
unsigned long clients_number = 0;


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


int main() {
//	int client_counter_current = 0;
//	int client_counter_recent = 0;
    int msqid;
    key_t key;
    size_t msg_length;
    msg my_msg;

    if ((key = ftok("./server", 0)) < 0) {
        perror("ftok");
        exit(-1);
    }
if ((msqid = msgget(key, 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        if (errno != EEXIST) {
            perror("Can't creat msgq");
            exit(-2);
        }
        if ((msqid = msgget(key, 0)) == -1) {
            perror("Can't find msgq");
            exit(-3);
        }
    }

    while (msgrcv(msqid, &my_msg, MSGSZ, SERVER_TYPE, 0) != -1) {
        printf("Message:\ntype=%ld\npid=%d\ntext=\"%s\"\n\n", my_msg.type, my_msg.info.pid, my_msg.info.text);

        if (strcmp(my_msg.info.text, "Connected") == 0) {
            add_client(my_msg.info.pid);
        } else if (strcmp(my_msg.info.text, "Disonnected") == 0) {
            remove_client(my_msg.info.pid);
        }
    }

    // failed to receive message, stop server
    perror("msgrcv");
    exit(-6);
}
