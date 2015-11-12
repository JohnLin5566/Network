/* client program */
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h> 
#define MAX_LINE 256		/* Maximum String Length Defined */
 
int main (int argc, char *argv[])
{
	int s, new_s;  
	char buf[MAX_LINE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char server_ip[INET6_ADDRSTRLEN];
	int len, pid;

	/* input argument error detection, and show the "usage" */
	if (argc != 3) {
	    fprintf(stderr,"usage: client hostname port\n");

	    exit(1);
	}

	/* reset the data from the hints, allow IPv4 or IPv6, and use TCP */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], "7331", &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all the results and connect to the first we can */
	for(p = servinfo; p != NULL; p = p->ai_next) {
		/* socket() function error detection, and output the error */
		if ((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		/* connect() function error detection, and output the error */
		if (connect(s, p->ai_addr, p->ai_addrlen) == -1) {
			close(s);
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
	inet_ntop(p->ai_family, (struct sockaddr *)p->ai_addr, server_ip, sizeof(server_ip));
	/* resulting string is copied to the buffer pointed to by dst */
	printf("client: connecting to %s\n", server_ip);
	/* all done with this structure */
	freeaddrinfo(servinfo);

	/* Get and send lines of text */
	if ((pid=fork())>0) {
		while (fgets(buf,sizeof(buf), stdin)) {
			buf[MAX_LINE-1]='\0';
			len=strlen(buf)+1;
			send (s, buf, len, 0) ;
		}
	} else {       
		while(len=recv(s,buf,sizeof(buf),0)){
			fputs(buf,stdout);
		}
	close(s);
	}  
}

