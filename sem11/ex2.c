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
//Client

int sendall (int s, char *buf, int len, int flags)
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

int main ()
{
	int sockfd;
	int n;
	int a;
	char sendline [1000] = "Cats", recvline[1000];
	struct sockaddr_in servaddr;
	
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket");
		exit(-1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(51000);
	servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	if((a = connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr))) < 0)
	{
		perror("connect");
		exit(-1);
	}
	printf("connect = %d\n", a);
	if ((n = sendall(sockfd, sendline, LEN, 0) ) < 0 ) {
		 perror("send");
		 exit(-1);
	}
	printf("sendall = %d\n", n);
	if ((n = recv(sockfd, recvline, LEN, 0) ) < 0 ) {
		 perror("recv");
		 exit(-1);
		 close(sockfd);
	}

	printf("%s", recvline);
	close(sockfd);
	
	return 0;
}
