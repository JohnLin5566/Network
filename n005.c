/* what the getaddrinfo function returns */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

int main (int argc, char *argv[])
{
	struct addrinfo *res = 0 ;

	getaddrinfo("localhost", NULL ,NULL,&res);
	
	printf("ai_flags -> %i\n", res->ai_flags) ;
	printf("ai_family -> %i\n", res->ai_family) ;
	printf("ai_socktype -> %i\n", res->ai_socktype) ;
	printf("ai_protocol -> %i\n", res->ai_protocol) ;
	printf("ai_addrlen -> %i\n", res->ai_addrlen) ;
	
	struct sockaddr_in* saddr = (struct sockaddr_in*)res->ai_addr;
	
	printf("ai_addr hostname ->  %s\n", inet_ntoa(saddr->sin_addr));
	
	freeaddrinfo(res);

	return 0 ;
}
