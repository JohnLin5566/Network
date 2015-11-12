/* use gethostbyaddr() function from IP to get the info */
#include<stdio.h>		/* printf */
#include<netdb.h>		/* gethostbyname, hostent */
#include<sys/socket.h>		

/* argc = argument count, the system would count the arguments in numbers.
 * argv[] = argument value, the parameter after the execute file, every strings is aparted
 * by blank, the first value is argv[0], argv[1], argv[2]..... */
int main(int argc, char *argv[])
{

//	struct hostent {
//		char  *h_name;            /* official name of host */
//		char **h_aliases;         /* alias list */
//		int    h_addrtype;        /* host address type */
//		int    h_length;          /* length of address */
//		char **h_addr_list;       /* list of addresses */
//	}
	struct hostent *host;	
	if (argc <2 ) return ; 
	
//	struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type);
	host = gethostbyaddr(argv[1], sizeof(argv[1]), AF_INET);

	if (host == (struct hostent*) NULL)
//		void herror(const char *s);
		herror("gethostbyaddr");
	else { 
	printf("name : %s\n", host->h_name);
	printf("alias : %s\n", host->h_aliases[0]);
	printf("type : %d\n", host->h_addrtype);
	printf("addr : %s\n", host->h_addr_list[0]);
	}
}
