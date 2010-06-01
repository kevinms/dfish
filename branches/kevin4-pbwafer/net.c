#include "net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

//#include <errno.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>

// Simulator Variables
char state;
unsigned int opt;

net_t *NET_socket_server(const char *node, const char *service)
{
	net_t *n;
	int rv;
	int yes = 1;
	struct addrinfo hints;     // Address info IP version agnostic
	struct addrinfo *servinfo; // Filled after a getaddrinfo()
	struct addrinfo *p;

	n = (net_t *)malloc(sizeof(*n));

	memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;         // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_DGRAM;      // UDP datagram sockets
	hints.ai_flags = AI_PASSIVE;         // fill in my IP for me (servers)

	if ((rv = getaddrinfo(node, service, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rv));
		return NULL;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((n->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(n->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			return NULL;
		}

		if (bind(n->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(n->sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return NULL;
	}

	// Store the sockaddr and length in the net_t
	n->addr = (struct sockaddr *)malloc(p->ai_addrlen);
	memcpy(n->addr, p->ai_addr, p->ai_addrlen);
	n->addrlen = p->ai_addrlen;

	// Can go ahead and free servinfo since it is not stored in net_t anymore
	freeaddrinfo(servinfo);

	return n;
}

net_t *NET_socket_client(const char *node, const char *service)
{
	net_t *n;
	int rv;
	struct addrinfo hints;     // Address info IP version agnostic
	struct addrinfo *servinfo; // Filled after a getaddrinfo()
	struct addrinfo *p;

	n = (net_t *)malloc(sizeof(*n));

	memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC;         // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_DGRAM;      // UDP datagram sockets

	if ((rv = getaddrinfo(node, service, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rv));
		return NULL;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((n->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: failed to bind socket\n");
		return NULL;
	}

	// Store the sockaddr and length in the net_t
	n->addr = (struct sockaddr *)malloc(p->ai_addrlen);
	memcpy(n->addr, p->ai_addr, p->ai_addrlen);
	n->addrlen = p->ai_addrlen;

	// Can go ahead and free servinfo since it is not stored in net_t anymore
	freeaddrinfo(servinfo);

	return n;
}

//TODO: Handle partial sends
int NET_send(net_t *n, fixedbuf_t *b)
{
	int numbytes;

	if ((numbytes = sendto(n->sockfd, b->buf, b->cursize, 0, n->addr, n->addrlen)) == -1) {
		perror("talker: sendto");
	}

	return numbytes;
}

//TODO: Handle partial recvs
//TODO: Make sure this is non-blocking so that we can use select()
int NET_recv(net_t *n, fixedbuf_t *b)
{
	int numbytes;

	//TODO: Need to store these in net_t *n somewhere, There server will need to
	//      keep track of all the clients who connect to them.  Will probably
	//      use a linked list at first but eventually switch over to a hash map
	struct sockaddr_storage addr;
	size_t addrlen;

	addrlen = sizeof(addr);
	if ((numbytes = recvfrom(n->sockfd, b->buf, b->maxsize , 0, (struct sockaddr *)&addr, &addrlen)) == -1) {
		perror("recvfrom");
	}

	return numbytes;
}

void NET_free(net_t *n)
{
	
}

void NET_sim(net_t *n)
{
	if(!n->state)
		return;

	if(n->opt & SIM_PL) {
	}
	if(n->opt & SIM_MDR) {
	}
	if(n->opt & SIM_MTU) {
	}
	if(n->opt & SIM_RTT) {
	}
	if(n->opt & SIM_DC) {
	}
}
