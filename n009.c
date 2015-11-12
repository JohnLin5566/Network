/* A stream socket client demo from Beej */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT "3490" 		// the port client will be connecting to 
#define MAXDATASIZE 100		// max number of bytes we can get at once 

/* check sockaddr is IPv4 or IPv6 */
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];

	/* input argument error detection, and show the "usage" */
	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

	/* reset the data from the hints, allow IPv4 or IPv6, and use TCP */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all the results and connect to the first we can */
	for(p = servinfo; p != NULL; p = p->ai_next) {
		/* socket() function error detection, and output the error */
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		/* connect() function error detection, and output the error */
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		/* If connect to the server, then jump out the for loop */
		break;
	}
	/* If struct addrinfo p is NULL, output the error */
	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	/* convert IPv4 or IPv6 addresses from binary to text form, and check the 
	 * address is IPv4 or IPv6 first */
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
	/* resulting string is copied to the buffer pointed to by dst */
	printf("client: connecting to %s\n", s);
	/* all done with this structure */
	freeaddrinfo(servinfo);


	/* recv() function error detection */
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    perror("recv");
	    exit(1);
	}
	/* the last postion of buf array is '\0' means the end of the array */
	buf[numbytes] = '\0';
	/* print the received from the server */
	printf("client: received '%s'\n",buf);
	close(sockfd);

	return 0;
}

