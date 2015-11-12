/* construct a file connect to the telnet */

#include<stdio.h>	/* print, exit, fpirntf */
#include<unistd.h>	/* write */
#include<netdb.h>	/* hint */
#include<errno.h>
#include<sys/socket.h>	/* recv, socket, connect, gai_strerror */
#include<string.h>	/* strerror */
#define MAXADDRLEN 256
#define BUFLEN 500

extern void exit();

void print_resp(int sockfd)
{
	printf("ready to print response\n");
	char buf[BUFLEN];
	int n, i;
	for (i=0; i<5; i++)
	{
		n=recv(sockfd, buf, BUFLEN, 0);
		write(STDOUT_FILENO, buf, n);
	}
}

int main(int argc, char *argv[])
{
	/* ai=addrinfo */
	struct addrinfo *ailist, *aip;
	/*It is possible many addrinfo from the server, so we use "hint" to filter, 
	 *and the type of "hint" is struct */
	struct addrinfo hint; 
	int sockfd, err;

	hint.ai_flags=0;		hint.ai_family=0;
	hint.ai_socktype=SOCK_STREAM;	hint.ai_protocol=0;
	hint.ai_addrlen=0;		hint.ai_canonname=NULL;
	hint.ai_addr=NULL;		hint.ai_next=NULL;


	/* generate a getaddrinfo */
	if ( (err=getaddrinfo("ptt.cc", "23", &hint, &ailist )) != 0)
	{
		printf("getaddrinfo error:%s\n", gai_strerror(err));
		exit(-1);
	}
	printf("getaddrinfo success\n");
	for (aip =ailist; aip !=NULL; aip=aip->ai_next)
	{
		/* generate a socket */
		if ( (sockfd = socket(aip->ai_family, SOCK_STREAM, 0 )) < 0){
			err = errno ;
		}
		
		printf("make socket success\n");

		/* connection */
		if (connect(sockfd, aip->ai_addr, aip->ai_addrlen) < 0 ){
			err = errno;
		} else {
			printf("socket connetc success\n");
			print_resp(sockfd);
			exit(0);
		}
	}

	fprintf(stderr, "cant connect to the target: %s\n", strerror(err));
	exit(1);
}
