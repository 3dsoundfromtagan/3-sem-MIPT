/*Сервер 
 * Сообщает о подключении клиента всем клиентам
 * Принимает сообщение от клиента N и отправляет его либо всем, либо клиенту K
 * Тип сообщения - номер клиента?! Сервер принимает тип 255, клиенты - i, посылка всем - 254
 * Задача 1. Заставить сервер писать сообщения о 
 * КАК НУМЕРОВАТЬ КЛИЕНТОВ? Client N (dis)connected???
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#define MSGSZ 128

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
	int client_counter_current = 0;
	int client_counter_recent = 0;
	int msqid;
	key_t key;
	size_t msg_length;
//	int i = 0;
	
	if ((key = ftok("./server", 0)) < 0) {
		perror("ftok");
		exit(-1);
	}
	
	if ((msqid = msgget(key, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
		if (errno!=EEXIST) {
			perror("Can't creat msgq");
			exit(-2);
		}
		else {
			if ( (msqid = msgget(key, 0)) < 0) {
				perror("Can't find msgq");
				exit(-3);
			}
		}
	}
	
/*	#ifdef SENDER
		// code for SENDER
		int n = atoi(argv[1]);
		msg* my_msg = (msg*)malloc((n + 1) * sizeof(msg));
	
		for (i = 0; i < n; i++) {
			(my_msg + i)->type = 1;
			strcpy((my_msg + i)->text, "I am in the queue");
			msg_length = strlen((my_msg + i)->text) + 1;
			if (msgsnd(msqid, (my_msg + i), msg_length, 0) < 0) {
				perror("msgsnd");
				exit(-4);
			}
		}
		
		(my_msg + n)->type = LAST_MESSAGE;
		msg_length = 0;
		if (msgsnd(msqid, (my_msg + n), msg_length, 0) < 0) {
			perror("msgsnd");
			exit(-5);
		}
		free(my_msg);
	#endif*/
	
	// code for RECEIVER	
	msg* my_msg = (msg*)malloc(sizeof(msg));
	
	while(1) {
		if (msgrcv(msqid, my_msg, MSGSZ, SERVER_TYPE, 0) < 0) {
			perror("msgrcv");
			exit(-6);
		}

		printf("Client %d: %s type = %ld\n", my_msg->info.pid, my_msg->info.text, my_msg->type);	
		//And now we have to send this message to all clients
		if (client_counter_current > client_counter_recent) {
			//Где-то тут ты проебался по disconnect
			//If new client appears
			my_msg->type = WRALL_TYPE;
			strcpy(my_msg->info.text, "New client was connected");
			//msg_length = (size_t)(strlen(my_msg->text) + 1);
			//msg_length = sizeof(my_msg);
			if (msgsnd(msqid, (my_msg), MSGSZ, 0) < 0) {
				perror("msgsnd");
				exit(-4);
			}
		}
		client_counter_recent = client_counter_current;
		/*if (my_msg->type == LAST_MESSAGE) {
			if (msgctl(msqid, IPC_RMID, NULL) < 0) {
				perror("msgctl");
				exit(-7);
			}
			exit(0);
		}*/
		
	}
	free(my_msg);
	
	return 0;
}
