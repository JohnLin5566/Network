/* A stream socket server demo from Beej */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>		/* waitpid, setsockopt */
#include <sys/socket.h>		/* setsockopt */
#include <netinet/in.h>		/* sturct sockaddr, INET6_ADDRSTRLEN */
#include <netdb.h>
#include <arpa/inet.h>		/*  */
#include <sys/wait.h>		/* waitpid */
#include <signal.h>		/* sigaction */
#define PORT "3490"  // the port users will be connecting to
#define BACKLOG 10	 // how many pending connections queue will hold


void sigchld_handler(int s){
	while(waitpid(-1, NULL, WNOHANG) > 0);
}
/* check is IPv4 or IPv6, and get it */
void *get_in_addr(struct sockaddr *sa){
	/* AF_INET means the sa_famliy is IPv4 */
	if (sa->sa_family == AF_INET) {
		/* return the IPv4 of sturct sockaddr_in */
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	/* If is not IPv4, then return the IPv6, sturct sockaddr_in6 */
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
	/* listen on sock_fd(socket file descriptor, new connection on new_fd */
	int sockfd, new_fd;
	struct addrinfo hints, *servinfo, *p;
	/* storage the connector's address information */
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;		/* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM;	/* Datagram socket */
	hints.ai_flags = AI_PASSIVE;		/* For wildcard IP address */
	/* error detection */
	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	/* loop through all the results and bind to the first we can */
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}
	/* all done with this structure */
	freeaddrinfo(servinfo);
	/* listen error detection */
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	/* reap all dead processes */
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	/* sigaction error detection */
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	printf("server: waiting for connections...\n");
	/* now the listen() is opening, just waiting the signal to accept */
	while(1) {
		sin_size = sizeof(their_addr);
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		/* accept error detection, and report */
		if (new_fd == -1) {
			perror("accept");
			continue;
		}
		/* convert IPv4 or IPv6 addresses from binary to text form, and check
		 * the address is IPv4 or IPv6 first */
		inet_ntop(their_addr.ss_family,	get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof(s));
		/* resulting string is copied to the buffer pointed to by dst */
		printf("server: got connection from %s\n", s);
		/* This is the child process */
		if (!fork()) { 
			/* child doesn't need the listener */
			close(sockfd); 
			/* send a message on a socket, and send() detection */
			if (send(new_fd, "Hello, world!", 13, 0) == -1){
				perror("send");
			}
			close(new_fd);
			exit(0);
		}
		/* parent process doesn't need this */
		close(new_fd);
	}
	return 0;
}

