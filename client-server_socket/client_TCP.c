#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
	int sockfd;
	int i;
 
	char sendline[1000], recvline[1000];
	struct sockaddr_in servaddr;

	if (argc != 2)
	{
		printf("insert IP!\n");
		exit(-1);
	}
	
	bzero(sendline, sizeof(sendline));
	bzero(recvline, sizeof(recvline));
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0) ) < 0 )
	{
		perror("socket");
		exit(-1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(62000);
	if (inet_aton(argv[1], &servaddr.sin_addr) == 0 )
	{
		printf("Invalid IP address\n");
		close(sockfd);
		exit(-1);
	}
	
	if(connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 )
	{
		perror("connect");
		close(sockfd);
		exit(-1);
	}
	
	printf("To break the connection enter quit\n");
	while(1)
	{
		printf("Insert string: ");
		fgets(sendline, sizeof(sendline), stdin);
		if (!strcmp(sendline, "quit\n"))
		{
			break;
		}
		
		if (write(sockfd, sendline, strlen(sendline) + 1) < 0)
		{
			perror("write");
			close(sockfd);
			exit(-1);
		}
		if (read(sockfd, recvline, sizeof(recvline) - 1) < 0)
		{
			perror("read");
			close(sockfd);
			exit(-1);
		}
	printf("Message received: %s", recvline);
	}
	close(sockfd);
	return 0;
}
