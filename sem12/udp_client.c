#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define LEN 1000
//UDP server
int main()
{
	int sockfd;
	int len, n;
	char sendline[LEN] = "kotiki\n";
	struct sockaddr_in servaddr, cliaddr;
	
	bzero(&cliaddr, sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(52000);
	cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		perror("socket");
		exit(-1);
	}

	if(bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0 )
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	//потiм розберемося з портами
	int i = 0;
	for (i = 0; i < 2; i++)
	{
		if (sendto(sockfd, sendline, strlen(sendline) + 1, 0, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
		{
			perror("sendto");
			close(sockfd);
			exit(-1);
		}
	}
	
	close(sockfd);
	return 0;
}

