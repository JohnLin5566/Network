/* create a socket */
/* function socket() creates a socket and returns a descriptor which can use
 * in other functions. */
#include<stdio.h>
#include<sys/socket.h>
int main()
{
	int socket_desc;

	if ( (socket_desc = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		printf("Could not create socket\n");
	}

	printf("socket_desc is %d\n", socket_desc);

	return 0;
}
