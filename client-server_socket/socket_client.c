#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>

#define MAX_NUM_MSG 100
#define MAX_TEXT 1000
#define MAX_NAME 100
#define SEM_NAME_1 "/sem1"
#define SEM_NAME_2 "/sem2"
sem_t sem;

struct msg_t
{
	char from[MAX_NAME];
	char to[MAX_NAME];
	char text[MAX_TEXT];
};

void correct (char* buf)
{
	int i = 0;
	int n = strlen(buf);
	for (i = 0; i < n - 2; i++)
	{
		buf[i] = buf[i + 1];
	}
	buf[n - 2] = '\0';
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

void* wait_messages(void* _arg)
{
	int sockfd = *((int *)_arg);
	int recv_len;
	struct msg_t msg;
	
	while (1)
	{
		if ((recv_len = recv(sockfd, &msg, sizeof(msg), 0)) <= 0)
		{
			perror("recv");
			exit(-1);
		}

		printf("[%s]: %s\n", msg.from, msg.text);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	int sockfd;
	pthread_t thread_id;
	struct msg_t msg;
	struct sockaddr_in serv_addr; 

	if(argc != 3)
	{
		printf("\n Usage: %s <ip of server> <nickname> \n",argv[0]);
		return 1;
	} 

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(-1);
	} 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(44700); 

	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0)
	{
		perror("inet_pton");
		close(sockfd);
		exit(-1);
	}

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("connect");
		close(sockfd);
		exit(-1);
	} 
	int result = pthread_create(
		&thread_id,
		(pthread_attr_t *)NULL, 
		wait_messages,
		&sockfd
	);
	
	if (result)
	{
		perror("pthread_create");
		exit(-1);
	}
	
	strcpy(msg.from, argv[2]);
	strcpy(msg.to, "ALL");
	strcpy(msg.text, "connected");
	if (send(sockfd, &msg, sizeof(msg), 0) < 0)
	{
		perror("send");
		close(sockfd);
		exit(-1);
	}
	printf("Console Chat 1.0\nTo change the status, enter 'status X' (X = 'passive' or 'active')\nThe first argument is the destination or ALL, the second is the message text\n");
	while (1)
	{
		scanf("%s", msg.to);
		if (!strcmp(msg.to, "quit"))
		{
			break;
		}
		fgets(msg.text, sizeof(msg.text), stdin);
		correct(msg.text);
		if (_send(sockfd, &msg, sizeof(msg), 0) < 0)
		{
			perror("send");
			close(sockfd);
			exit(-1);
		}
	}

	close(sockfd);
	return 0;
}
