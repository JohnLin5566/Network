/* Client program, The programs are an echo server and client for UDP datagrams. */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		/* write, read */
#include <string.h>		/* strlen */
#define BUF_SIZE 500
int main(int argc, char *argv[]){
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s, j;
	size_t len;
	ssize_t nread;
	char buf[BUF_SIZE];
	/* If argc is not correct, output the "Usage" */
	if (argc < 3) {
		fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	/* Obtain address(es) matching host/port */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;          /* Any protocol */
	/* get addrinfo, node=argv[1], server=argv[2] */
	s = getaddrinfo(argv[1], argv[2], &hints, &result);
	/* error detection */
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	/* getaddrinfo() returns a list of address structures. Try each address 
	 * until we successfully connect(2). If socket(2) (or connect(2)) fails,
	 * we (close the socket and) try the next address. */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		/* fail situation, try the next address */
		if (sfd == -1){
			continue;
		}
		/* success, break from for loop */
		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1){
			break;
		}
		/* If fail, then close this socket, try the next address */
		close(sfd);
	}	
	/* If no address succeeded, output the error */
	if (rp == NULL) {
        	fprintf(stderr, "Could not connect\n");
	        exit(EXIT_FAILURE);
	}
	/* Send remaining command-line arguments as separate datagrams, and read 
	 * responses from server */
	for (j = 3; j < argc; j++) {
		/* calculate the string length, and +1 for terminating null byte */
		len = strlen(argv[j]) + 1;
		/* If the length greater than the BUF_SIZE, output error */
		if (len + 1 > BUF_SIZE) {
			fprintf(stderr,"Ignoring long message in argument %d\n", j);
			continue;
		}
		/* write() error detection, if the length not equate argv[3] */
		if (write(sfd, argv[j], len) != len) {
			fprintf(stderr, "partial/failed write\n");
			exit(EXIT_FAILURE);
		}
		/* read up to count bytes from file descriptor fd into the buffer 
		 * starting at buf. */
		nread = read(sfd, buf, BUF_SIZE);
		/* read() error detection */
		if (nread == -1) {
			perror("read");
			exit(EXIT_FAILURE);
		}
		printf("Received %zd bytes: %s\n", nread, buf);
	}
	exit(EXIT_SUCCESS);
}
