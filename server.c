/* server program */
#include<stdio.h>
#include<time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<string.h>
#include<stdlib.h> 
#define MAX_PENDING 5		/* Maximum number of clients */
#define MAX_LINE 256		/* Maximum String Length Defined */
 
int main (int argc, char *argv[]){

	/* listen on sock_fd(socket file descriptor, new connection on new_fd */
	int s, new_s;
	struct addrinfo hints, *servinfo, *p;
	/* storage the connector's address information */
	struct sockaddr_storage their_addr;
	int len,pid,ptr;
	int yes=1;
	int rv;
	char buf[MAX_LINE];
	char *welcome_message1, *welcome_message2, *win_message;


	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		/* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM;	/* Datagram socket */
	hints.ai_flags = AI_PASSIVE;		/* For wildcard IP address */

	/* argc error detection, output the "Usage" */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* get the address info */
	if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	/* loop through all the results and bind to the first we can */
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(s, p->ai_addr, p->ai_addrlen) == -1) {
			close(s);
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
	if (listen(s, MAX_PENDING) == -1) {
		perror("listen");
		exit(1);
	}

	/* Wait for a connection, then receive and print text   */
	while(1) {
		ptr=sizeof(their_addr);
		if ((new_s = accept(s, (struct sockaddr *)&their_addr, &ptr)) < 0) {
        	    perror("error: accept");
        	    exit(1);
        	}
		/* child process is forked to recieve and send text */
	        if((pid=fork())>0){

			/* use rand() to create a random number in a range */
			int bingo_num;
			bingo_num = (rand() % 10);

			/* recieve messages from client */
			while(len=recv (new_s, buf, sizeof(buf), 0)){
				fputs(buf, stdout);
			
				/* if the recieve number is equal bingo number */
				if ( bingo_num==atoi(buf) ){
					
					/* If answer is right, output this line */
					win_message = "You win the game!!\n";
					if( send(new_s , win_message , strlen(win_message) , 0) < 0){
						puts("Send failed");
						return 1;
					}
				}
			}
			close(new_s);
		} else {
			/* input the chat conversation */   
			while(fgets(buf,sizeof(buf),stdin)){
				buf[MAX_LINE-1]='\0'; 
				len=strlen(buf)+1;
				send(new_s,buf,len,0); 
			} 
		}   
	}
     
}
