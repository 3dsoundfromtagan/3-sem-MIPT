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

int main() {
	int sockfd, newsockfd;
	int clilen;
	int n, a;
	char line[1000];
	struct sockaddr_in servaddr, cliaddr;
	
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		perror("socket");
		exit(-1);
	}
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(64000);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if(bind(sockfd, (struct sockaddr * ) &servaddr, sizeof(servaddr)) < 0 ) {
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	
	if(listen(sockfd, 10)) {
		perror("listen");
		close(sockfd);
		exit(-1);
	}
	
	while(1) {
		clilen = sizeof(cliaddr);
		if ( (newsockfd = accept(sockfd, (struct sockaddr* ) &cliaddr, &clilen)) < 0) {
			perror("accept");
			close(sockfd);
			exit(-1);
		}
		
		a = fork();
		
		if (a < 0) {
			perror("fork");
			exit (-1);
		}

		if (a == 0) {
			//CHILD
			
			close(sockfd);
			while ( (n = read(newsockfd, line, sizeof(line)-1)) > 0) {
				printf("Input data: %s", line);
				if ( (n = write(newsockfd, line, strlen(line)+1)) < 0) {
					perror("write");
					close(sockfd);
					close(newsockfd);
					exit(-1);
				}
			}
			if(n < 0) {
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
	
	return 0;
}
