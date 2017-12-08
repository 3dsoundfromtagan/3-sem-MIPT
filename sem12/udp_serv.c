#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
//UDP server
int main()
{
	int sockfd;
	int i = 2;
	int clilen, n;
	char line[1000];
	struct sockaddr_in servaddr, cliaddr;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0 )
	{
		perror("socket");
		exit(-1);
	}

	if(bind(sockfd, (struct sockaddr * ) &servaddr, sizeof(servaddr)) < 0 )
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	while(i)
	{
		clilen = sizeof(cliaddr);

		if ((n = recvfrom(sockfd, line, 1001, 0, (struct sockaddr* ) (socklen_t*)&cliaddr, &clilen) ) < 0 )
		{
			perror("receive");
			exit(-1);
			close(sockfd);
		}
		i--;
		printf("%s", line);
	}
 
	return 0;
}
