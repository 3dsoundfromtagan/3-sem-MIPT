/*Сервер
 * Сообщает о подключении клиента всем клиентам
 * Принимает сообщение от клиента N и отправляет его либо всем, либо клиенту K
*/

#include "protocol.h"
//int index = 0;		//index of msg_shm
pid_t client_pids[MAX_CLIENTS];
unsigned long clients_number = 0;
int msqid;

void add_client(pid_t pid, msg_shm* message_counter) {
    client_pids[clients_number] = pid;
    (message_counter + clients_number)->pid = pid;
    (message_counter + clients_number)->counter = 0;
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

void send_all(const char *const message, pid_t pid, enum command_type command, msg_shm* message_counter) {
    msg my_msg;
    strcpy(my_msg.info.text, message);
    my_msg.info.pid_from = pid;
    my_msg.info.comm_type = command;
    for (int i = 0; i < clients_number; i++) {
		if (pid != client_pids[i]) {
			my_msg.type = client_pids[i];
			(message_counter + clients_number)->pid = pid;
			(message_counter + clients_number)->counter++;
			send_message(&my_msg);
		}
	}
}

void send_todef (const char *const message, pid_t pid, enum command_type command, msg_shm* message_counter) {
	msg my_msg;
    strcpy(my_msg.info.text, message);
    my_msg.info.pid_from = pid;
    my_msg.info.comm_type = command;
	my_msg.type = my_msg.info.pid_to;
	for (int i = 0; i < clients_number; i++) {
		if ((message_counter + i)->pid == pid) {
			(message_counter + i)->pid = pid;
			(message_counter + i)->counter++;
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
    send_all(message, pid, command, NULL);
}

void send_disconnect_message(pid_t pid, enum command_type command) {
    char message[MSGSZ];
    sprintf(message, "Client with pid = %d disconnected", pid);
    send_all(message, pid, command, NULL);
}

int main() {
    msg my_msg;	//Message to be sen and receive
    msg_shm message_counter[MAX_CLIENTS]; //Massive of tructures to count messages
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
                send_all(my_msg.info.text, my_msg.info.pid_from, my_msg.info.comm_type, message_counter);
                break;
            case WRITE:
                send_todef(my_msg.info.text, my_msg.info.pid_from, my_msg.info.comm_type, message_counter);
                break;
            case CONNECT:
                add_client(my_msg.info.pid_from, message_counter);
                send_connect_message(my_msg.info.pid_from, my_msg.info.comm_type);
                for_connect_client(my_msg.info.pid_from, my_msg.info.comm_type);
                break;
            case DISCONNECT:
                remove_client(my_msg.info.pid_from);
                send_disconnect_message(my_msg.info.pid_from, my_msg.info.comm_type);
                break;
        }
    }
    // failed to receive message, stop server
    perror("msgrcv");
    exit(-6);
}
