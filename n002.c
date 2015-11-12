/* use socket client to construct a TCP connection to server */
#include<sys/stat.h>
#include<stdlib.h>		/* exit */
#include<stdio.h>		/* perror */
#include<string.h>		/* bzero */
#include<fcntl.h>
#include<unistd.h>		/* read */
#include<sys/types.h>		/* socket, recv, send */
#include<sys/socket.h>		/* connect, recv, send */
#include<netinet/in.h>		/* sockaddr_in */
#include<arpa/inet.h>		/* htons */
#define PORT 1234
#define SERVER_IP "127.0.0.1"

int main()
{
	int s;		
	char buffer[256];
	struct sockaddr_in addr;

	/* AF_INET, address family internet protocol, 
	 * SOCK_STREAM, provides swquenced, reliable, two-way, connection-based byte streams. */
	if (( s = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket");
		exit(1);
	}

	/* write sockaddr_in structure */
	/* bzero, write zero-valued bytes; sizeof, return the size of the byte of the variable */
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	/* The htons() functions converts the unsigned short integer hostshort from host byte
	 * order to network byte oder */
	addr.sin_port = htons(PORT);
	/* see " struct sockaddr_in " */
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	/* connection */
	/* sockaddr_in : 4 argument, sockaddr : 2 argument *, so use (struct sockaddr *)
	 * to convert */
	if (connect(s,(struct sockaddr *) &addr, sizeof(addr)) < 0){
		perror("connect");
		exit(1);
	}

	/* receive infomation from server */
	recv(s, buffer, sizeof(buffer), 0);
	printf("%s\n", buffer);

	while(1){
		bzero(buffer, sizeof(buffer));
		/* get the strings from standard input(STDIN_FILENO); 
		 * read, read from a file descriptor */
		read(STDIN_FILENO, buffer, sizeof(buffer));
		/* send the strings to server */
		if (send(s, buffer, sizeof(buffer), 0) < 0){
		/* perror, print a system error message */
		perror("send");
		exit(1);
		}
	}
}
