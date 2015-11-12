/* show IP addresses for a host given on the command line */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
int main(int argc, char *argv[])
{
	struct addrinfo hints, *res, *p;
	int status;
	char ipstr[INET6_ADDRSTRLEN];

	if (argc != 2) {
		fprintf(stderr,"usage: showip hostname\n");
		return 1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
	hints.ai_socktype = SOCK_STREAM;
	
//	int getaddrinfo(const char *node,	// e.g. "www.example.com" or IP
//        	const char *service,		// e.g. "http" or port number
//		const struct addrinfo *hints,
//		struct addrinfo **res);

//	struct addrinfo {
//		int              ai_flags;	//This field specifies additional options
//		int              ai_family;	//AF_INET, AF_INET6 or AF_UNSPEC
//		int              ai_socktype;	//SOCK_STREAM or SOCK_DGRAM.
//		int              ai_protocol;	//the protocol for the returned socket addresses.
//		socklen_t        ai_addrlen;
//		struct sockaddr *ai_addr;
//		char            *ai_canonname;
//	        struct addrinfo *ai_next;
//      };

	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		return 2;
	}

	printf("IP addresses for %s:\n\n", argv[1]);

//	struct sockaddr_in {
//		short            sin_family;   // e.g. AF_INET
//		unsigned short   sin_port;     // e.g. htons(3490)
//		struct in_addr   sin_addr;     // see struct in_addr, below
//      char             sin_zero[8];  // zero this if you want to
//	};
//		struct in_addr {
//		unsigned long s_addr;  // load with inet_aton()
//	};

	for(p = res;p != NULL; p = p->ai_next) {
		void *addr;
	        char *ipver;

		/* get the pointer to the address itself, different fields in IPv4 and IPv6 */
                if (p->ai_family == AF_INET) { 
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
                addr = &(ipv4->sin_addr);
                ipver = "IPv4";
                } else { 
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
                addr = &(ipv6->sin6_addr);
                ipver = "IPv6";
	}

//	inet_ntop - convert IPv4 and IPv6 addresses from binary to text form
//	const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
//	
//	converts the network address structure src in the af address family into a 
//	character string.  The resulting string is copied to the buffer pointed to 
//	by dst, which must be a non-null pointer.  The number of bytes available in
//	this buffer in the argument size.

	/* convert the IP to a string and print it */
	inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
	printf("  %s: %s\n", ipver, ipstr);

//	The freeaddrinfo() function frees the memory that was allocated for
//      the dynamically allocated linked list res.

	/* free the linked list */
	freeaddrinfo(res);

	return 0;
	}
}
