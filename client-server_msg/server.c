/*Сервер
 * Сообщает о подключении и отключении клиента всем клиентам
 * Принимает сообщение от клиента N и отправляет его либо всем, либо клиенту K
*/
//<<<<<<< HEAD

#include "protocol.h"
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

void send_message(msg *my_msg) {
    if (msgsnd(msqid, my_msg, MSGSZ, 0) < 0) {
        perror("msgsnd");
        exit(-4);
    }
}

void send_all(const char *const message, pid_t pid, enum command_type command) {
    msg my_msg;
    strcpy(my_msg.info.text, message);
    my_msg.info.pid_from = pid;
    my_msg.info.comm_type = command;
    for (int i = 0; i < clients_number; i++) {
		if (pid != client_pids[i]) {
			my_msg.type = client_pids[i];
			send_message(&my_msg);
		}
	}
}

void for_connect_client(pid_t pid, enum command_type command) {
	//notifies the new client about already connected
    char message[MSGSZ];
    msg my_msg;
    my_msg.info.comm_type = command;
    for(int i =0; i < clients_number; i++) {
        if(pid != client_pids[i])
        {
            sprintf(message, "Client with PID = %d is already connected", client_pids[i]);
            my_msg.type = pid;
            strcpy(my_msg.info.text, message);
            send_message(&my_msg);
        }
    }
}

void send_connect_message(pid_t pid, enum command_type command) {
    char message[MSGSZ];
    sprintf(message, "Client with PID = %d connected", pid);
    send_all(message, pid, command);
}

void send_disconnect_message(pid_t pid, enum command_type command) {
    char message[MSGSZ];
    sprintf(message, "Client with pid = %d disconnected", pid);
    send_all(message, pid, command);
}

int main() {
    msg my_msg;	//Message to be send and receive
    key_t key;
    key = get_key_or_exit();
    msqid = get_msqid_or_exit(key);
    printf("Started server with msqid = %d, pid = %d, key = %d\n", msqid, getpid(), key);

    ssize_t ret;
    while ((ret = msgrcv(msqid, &my_msg, MSGSZ, SERVER_TYPE, 0)) != -1) {
        printf("Message:\ntype=%ld\ncomm_type=%d\npid=%d\ntext=\"%s\"\n\n", my_msg.type, my_msg.info.comm_type,
               my_msg.info.pid_from, my_msg.info.text);

        switch (my_msg.info.comm_type) {
            case WRITE_ALL:
                send_all(my_msg.info.text, my_msg.info.pid_from, my_msg.info.comm_type);
                break;
            case WRITE:
				my_msg.type = my_msg.info.pid_to;
				send_message(&my_msg);
                break;
            case CONNECT:
                add_client(my_msg.info.pid_from);
                send_connect_message(my_msg.info.pid_from, my_msg.info.comm_type);
                for_connect_client(my_msg.info.pid_from, my_msg.info.comm_type);
                break;
            case DISCONNECT:
                remove_client(my_msg.info.pid_from);
                send_disconnect_message(my_msg.info.pid_from, my_msg.info.comm_type);
                break;
        }
    }
=======
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

>>>>>>> ca241d01634345094bcb0e67bb42d417a88f0b07
    // failed to receive message, stop server
    perror("msgrcv");
    exit(-6);
}
