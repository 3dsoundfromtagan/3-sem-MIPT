#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>

#define BUF_SIZE	64

int main() {
	int sockfd, newsockfd;
	int clients_number = 0;
	int clilen;
	int n, a;
	char line[1000];
	struct sockaddr_in servaddr, cliaddr;
	
	//char buf[BUF_SIZE];
	key_t k = ftok("./server_TCP", 1);
	if (k == -1)
	{
		perror("ftok");
		exit(-2);
	}
	const size_t sz = BUF_SIZE * sizeof(int);
	int s = shmget(k, sz, 0666|IPC_CREAT);
	if (s < 0)
	{
		perror("shmget");
		exit(-3);
	}
	int* addr = (int*)shmat(s, NULL, 0);
	if (addr == (int*)(-1))
	{
		perror("shmat");
		exit(-4);
	}
	
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
	{
		perror("socket");
		exit(-1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(62000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	
	if (listen(sockfd, 10))
	{
		perror("listen");
		close(sockfd);
		exit(-1);
	}
	
	while(1)
	{
		clilen = sizeof(cliaddr);
		if ((newsockfd = accept(sockfd, (struct sockaddr* ) &cliaddr, (socklen_t*)&clilen)) < 0)
		{
			perror("accept");
			close(sockfd);
			exit(-1);
		}
		
		clients_number++;
		*(addr + sizeof(int) * clients_number) = newsockfd;
		
		a = fork();
		
		if (a < 0)
		{
			perror("fork");
			exit (-1);
		}

		if (a == 0)
		{
			//CHILD
			
			close(sockfd);
			while ((n = read(newsockfd, line, (size_t) (sizeof(line) - 1))) > 0)
			{
				printf("newsockfd = %d, addr1 = %d, addr2 = %d\n", newsockfd, *(addr + sizeof(int) * 1), *(addr + sizeof(int) * 2));
				printf("Input data: %s", line);
				if ((n = write(*(addr + sizeof(int) * 2), line, (size_t) (strlen(line) + 1))) < 0)
				{
					perror("write");
					close(sockfd);
					close(newsockfd);
					exit(-1);
				}
			}
			if(n < 0)
			{
				perror("read");
				close(sockfd);
				close(newsockfd);
				exit(-1);
			}
			close(newsockfd);
			exit(1);
		}
		
		if (a > 0) {
			//PARENT
			close(newsockfd);
		}
		else {
			wait(NULL);
		}
	}
	
	if (shmdt(addr) == -1)
	{
		perror("shmdt");
		exit(-5);
	}
	if (shmctl(s, IPC_RMID, 0) == -1)
	{
		perror("shmctl");
		exit(-6);
	}
	
	return 0;
}
