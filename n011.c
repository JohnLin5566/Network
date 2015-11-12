/* Connect a socket to a server */
#include<stdio.h>		/* puts */
#include<sys/socket.h>		/* connect */
#include<arpa/inet.h>		/* inet_addr */
int main()
{
	int socket_desc;
	struct sockaddr_in server;

	/* create a socket */
	if ( (socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		printf("Could not create socket\n");
	}

	printf("socket_desc is %d\n", socket_desc);

	server.sin_addr.s_addr = inet_addr(" 64.233.191.255");	/* node */
	server.sin_family = AF_INET;				/* IPv4 */
	server.sin_port = htons(80);				/* port */

	/* connect to remote server */
	if ( connect(socket_desc, (struct sockaddr *)&server, sizeof(server) ) <0){
		puts("connect error\n");
		return 1;
	}

	puts("connected\n");
	return 0;
}
