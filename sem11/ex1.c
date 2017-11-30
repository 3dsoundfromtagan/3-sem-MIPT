#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#define LEN 5
//Server
int main ()
{
	int sockfd, newsockfd;
	int clilen;
	int n;
	char line[LEN + 1];
	struct sockaddr_in servaddr, cliaddr;
	
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket");
		exit(-1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	if (bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)))
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	
	if (listen(sockfd, 1) < 0)
	{
		perror("listen");
		close(sockfd);
		exit(-1);
	}
	
	clilen = sizeof(cliaddr);
	if ( (newsockfd = accept(sockfd, (struct sockaddr* ) &cliaddr, (socklen_t*)&clilen)) < 0)
	{
		perror("accept");
		close(sockfd);
		exit(-1);
	}
	printf("accept = %d,\naddr = %d\n", newsockfd, cliaddr.sin_addr.s_addr);
	if ((n = recv(newsockfd, line, LEN, 0) ) < 0 ) {
		 perror("recv");
		 exit(-1);
		 close(sockfd);
	}

	printf("%s", line);

	if ((n = send(newsockfd, line, LEN, 0) ) < 0 ) {
		 perror("send");
		 exit(-1);
	}
	
		close(sockfd);
		close(newsockfd);
		exit(-1);
	
	
	return 0;
}
