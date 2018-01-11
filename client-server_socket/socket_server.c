#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define MAX_TEXT 1000
#define MAX_NAME 100

struct msg_t
{
	char from[MAX_NAME];
	char to[MAX_NAME];
	char text[MAX_TEXT];
};

struct user_t
{
	char name[MAX_NAME];
	int connfd, idx, authorized;
	int is_alive;
	int status;
};

struct user_t users[100];
int online_users;

void remove_client(int idx)
{
	close(users[idx].connfd);
	users[idx].is_alive = 0;
}

int _send (int s, void *buf, int len, int flags)
{
	int total = 0;
	int n;
	
	while(total < len)
	{
		n = send(s, buf + total, len - total, flags);
		if (n == -1)
		{
			break;
		}
		total += n;
	}
	return (n == -1 ? -1 : total);
}

void send_message (struct msg_t msg, int idx)
{
	int i = 0, j = 0;
	for (i = 0; i < online_users; i++) 
	{
		if ((strcmp(msg.to, users[i].name) == 0) && (users[i].is_alive == 1))
		{
			j = _send(users[i].connfd, &msg, sizeof(msg), MSG_CONFIRM);
			if (j < 0)
			{
				perror("send");
				close(users[i].connfd);
				exit(-1);
			}
			printf("[%s] send %d bytes to %s\n", msg.from, j, users[i].name);
		}
		
	}
}

void sendall (struct msg_t msg, int idx)
{
	int i = 0, j = 0;;
	for (i = 0; i < online_users; i++) 
	{
		printf("i = %d\n", i);
		printf("[%s] is alive = %d, active = %d\n", users[i].name, users[i].is_alive, users[i].status);
		if ((i != idx) && (users[i].is_alive == 1) && (users[i].status == 1))
		{
			j = _send(users[i].connfd, &msg, sizeof(msg), MSG_CONFIRM);
			if (j < 0)
			{
				perror("send");
				close(users[i].connfd);
				exit(-1);
			}
			printf("[%s] send %d bytes to %s\n", msg.from, j, users[i].name);
		}
	}
}

void* process_client(void* _arg)
{
	int idx = *((int *)_arg);
	int i;
	struct msg_t msg;

	printf("started thread %d, connfd=%d\n", idx, users[idx].connfd);
	users[idx].status = 1;
	while (1)
	{
		printf("size=%lud\n", sizeof(msg));
		i = recv(users[idx].connfd, (void*)&msg, (size_t)sizeof(msg), 0);
		if (i < 0)
		{
			perror("recv");
			exit(-1);
		}
		if (i == 0)
		{
			printf("%s disconnected\n", users[idx].name);
			remove_client(users[idx].idx);
			strcpy(msg.text, "disconnected");
			sendall(msg, idx);
			return NULL;
		}
		if (users[idx].authorized == 0)
		{
			strcpy(users[idx].name, msg.from);
			users[idx].authorized = 1;
		}
		if (strcmp(msg.to, "ALL") == 0)
		{
			sendall(msg, idx);
		}
		if (strcmp(msg.to, "status") == 0)
		{
			printf("comparing; recieved:%s\n", msg.text);
			if (strcmp(msg.text, "passive") == 0)
			{
				users[idx].status = 0;
				printf("status = %d", users[idx].status);
			}
			if (strcmp(msg.text, "active") == 0)
			{
				users[idx].status = 1;
				printf("status = %d", users[idx].status);
			}
		}
		else
		{
			send_message(msg, idx);
		}
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	online_users = 0;
	int listenfd;
	struct sockaddr_in serv_addr, clt_addr;
	pthread_t thread_id[1000];

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(-1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(44700); 

	if (bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("bind");
		close(listenfd);
		exit(-1);
	}
	if (listen(listenfd, 10) < 0)
	{
		perror("listen");
		close(listenfd);
		exit(-1);
	}

	while (1)
	{
		int clt_addr_len = sizeof(clt_addr);
		if ((users[online_users].connfd = accept(listenfd, (struct sockaddr*)&clt_addr, (socklen_t*)&clt_addr_len)) < 0)
		{
			perror("accept");
			close(listenfd);
			exit(-1);
		}
		printf("%s connected\n", inet_ntoa(clt_addr.sin_addr));
		users[online_users].idx = online_users;
		users[online_users].authorized = 0;
		users[online_users].is_alive = 1;
		int result = pthread_create(
			&thread_id[online_users],
			(pthread_attr_t *)NULL, 
			process_client,
			&(users[online_users].idx)
		);

		if (result)
		{
			perror("pthread_create");
			exit(-1);
		}

		online_users++;
		printf("online_users=%d\n", online_users);
	}
}
