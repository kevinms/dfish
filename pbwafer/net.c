#include "net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

//#include <errno.h>
//#include <netinet/in.h>

//TODO:URGENT: Get the reliability layer working
//TODO:URGENT: Get KEEPALIVE msg type working to test the reliability layer
//TODO:URGENT: Get a basic flow control algorithm working based of a few metrics

void NET_init()
{
	
}

net_t *NET_socket_server(const char *node, const char *service)
{
	net_t *n;
	int rv;
	int yes = 1;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct addrinfo *p;

	n = (net_t *)malloc(sizeof(*n));

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(node, service, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rv));
		return NULL;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		printf("haha\n");
		printf("%d == %d\n",p->ai_family, AF_INET);
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

	// Set the socket to non-blocking
	fcntl(n->sockfd, F_SETFL, fcntl(n->sockfd, F_GETFL)|O_NONBLOCK);

	// Store the sockaddr and length in the net_t
	memcpy(&n->addr, p->ai_addr, p->ai_addrlen);
	n->addrlen = p->ai_addrlen;

	NET_print(n);

	// Can go ahead and free servinfo since it is not stored in net_t anymore
	freeaddrinfo(servinfo);

	return n;
}

net_t *NET_socket_client(const char *node, const char *service)
{
	net_t *n;
	int rv;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct addrinfo *p;

	n = (net_t *)malloc(sizeof(*n));

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

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

	// Set the socket to non-blocking
	fcntl(n->sockfd, F_SETFL, fcntl(n->sockfd, F_GETFL)|O_NONBLOCK);

	// Store the sockaddr and length in the net_t
	memcpy(&n->addr, p->ai_addr, p->ai_addrlen);
	n->addrlen = p->ai_addrlen;

	NET_print(n);

	// Can go ahead and free servinfo since it is not stored in net_t anymore
	freeaddrinfo(servinfo);

	return n;
}

//TODO: Handle partial sends
int NET_send(net_t *n, fixedbuf_t *b)
{
	int numbytes;

	NET_print(n);

	if ((numbytes = sendto(n->sockfd, b->buf, b->cursize, 0, (struct sockaddr *)&n->addr, n->addrlen)) == -1) {
		perror("talker: sendto");
	}

	if(numbytes > 0)
		printf("numbytes sent: %d\n",numbytes);

	return numbytes;
}

//TODO: Handle partial recvs
//TODO: Make sure this is non-blocking so that we can use select(): should be good now!
int NET_recv(net_t *n, fixedbuf_t *b)
{
	int numbytes;

	buf_clear(b);
	if ((numbytes = recvfrom(n->sockfd, b->buf, b->maxsize , 0, (struct sockaddr *)&n->addr, &n->addrlen)) == -1) {
		//perror("recvfrom");
	}

	if(numbytes <= 0)
		return numbytes;

	NET_print(n);

	printf("numbytes recved: %d\n",numbytes);
	buf_clear(b);
	b->cursize = numbytes;

	return numbytes;
}

void NET_copy(net_t *dest, net_t *src)
{
	dest->sockfd = src->sockfd;
	memcpy((void *)&(dest->addr), (void *)&src->addr, src->addrlen);
	dest->addrlen = src->addrlen;
}

int NET_ipcmp(net_t *n1, net_t *n2)
{
	int rv = 1;

	// IPv4 use sockaddr_in
	if(((struct sockaddr *)&n1->addr)->sa_family == AF_INET && ((struct sockaddr *)&n2->addr)->sa_family == AF_INET) {
		if(((struct sockaddr_in *)&n1->addr)->sin_addr.s_addr == ((struct sockaddr_in *)&n2->addr)->sin_addr.s_addr)
			rv = 0;
	}
	// IPv6 use sockaddr_in6
	else if(((struct sockaddr *)&n1->addr)->sa_family == AF_INET6 && ((struct sockaddr *)&n2->addr)->sa_family == AF_INET6) {
		if( !strncmp( (char *)((struct sockaddr_in6 *)&n1->addr)->sin6_addr.s6_addr, (char *)((struct sockaddr_in6 *)&n2->addr)->sin6_addr.s6_addr, 16) )
			rv = 0;
	}

	return rv;
}

int NET_portcmp(net_t *n1, net_t *n2)
{
	int rv = 1;
	unsigned short p1;
	unsigned short p2;

	// IPv4 use sockaddr_in
	if(((struct sockaddr *)&n1->addr)->sa_family == AF_INET && ((struct sockaddr *)&n2->addr)->sa_family == AF_INET) {
		p1 = htons(((struct sockaddr_in *)&n1->addr)->sin_port);
		p2 = htons(((struct sockaddr_in *)&n2->addr)->sin_port);
		if(p1 == p2)
			rv = 0;
	}
	// IPv6 use sockaddr_in6
	else if(((struct sockaddr *)&n1->addr)->sa_family == AF_INET6 && ((struct sockaddr *)&n2->addr)->sa_family == AF_INET6) {
		p1 = htons(((struct sockaddr_in6 *)&n1->addr)->sin6_port);
		p2 = htons(((struct sockaddr_in6 *)&n2->addr)->sin6_port);
		if(p1 == p2)
			rv = 0;
	}

	return rv;
}

void NET_print(net_t *n)
{
	char ip[INET6_ADDRSTRLEN];
	unsigned short port;

	printf("IP: ");

	// IPv4 use sockaddr_in
	if(((struct sockaddr *)&n->addr)->sa_family == AF_INET) {
		if(inet_ntop(AF_INET, &(((struct sockaddr_in *)&n->addr)->sin_addr), ip, INET6_ADDRSTRLEN) != NULL)
			printf("%s", ip);
		port = htons(((struct sockaddr_in *)&n->addr)->sin_port);
		printf(":%d\n", port);
	}
	// IPv6 use sockaddr_in6
	else if(((struct sockaddr *)&n->addr)->sa_family == AF_INET6) {
		inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)&n->addr)->sin6_addr.s6_addr), ip, INET6_ADDRSTRLEN);
		printf("%s", ip);
		port = htons(((struct sockaddr_in6 *)&n->addr)->sin6_port);
		printf(":%d\n", port);
	}
	// Address family unkown
	else {
		printf("Address family unkown!\n");
	}
}

char *NET_get_ip(net_t *n)
{
	char ip[INET6_ADDRSTRLEN];

	// IPv4 use sockaddr_in
	if(((struct sockaddr *)&n->addr)->sa_family == AF_INET) {
		if(inet_ntop(AF_INET, &(((struct sockaddr_in *)&n->addr)->sin_addr), ip, INET6_ADDRSTRLEN) != NULL)
			return strdup(ip);
	}
	// IPv6 use sockaddr_in6
	else if(((struct sockaddr *)&n->addr)->sa_family == AF_INET6) {
		if(inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)&n->addr)->sin6_addr.s6_addr), ip, INET6_ADDRSTRLEN) != NULL)
			return strdup(ip);
	}
	// Address family unkown
	printf("Address family unkown!\n");
	return NULL;
}

unsigned short NET_get_port(net_t *n)
{
	unsigned short port;

	// IPv4 use sockaddr_in
	if(((struct sockaddr *)&n->addr)->sa_family == AF_INET) {
		port = htons(((struct sockaddr_in *)&n->addr)->sin_port);
		return port;
		printf(":%d\n", port);
	}
	// IPv6 use sockaddr_in6
	else if(((struct sockaddr *)&n->addr)->sa_family == AF_INET6) {
		port = htons(((struct sockaddr_in6 *)&n->addr)->sin6_port);
		return port;
		printf(":%d\n", port);
	}
	// Address family unkown
	else {
		printf("Address family unkown!\n");
		return -1;
	}
}

void NET_free(net_t *n)
{
	
}

void NET_sim(net_t *n)
{
	if(!n->ns.state)
		return;

	if(n->ns.opt & SIM_PL) {
	}
	if(n->ns.opt & SIM_MDR) {
	}
	if(n->ns.opt & SIM_MTU) {
	}
	if(n->ns.opt & SIM_RTT) {
	}
	if(n->ns.opt & SIM_DC) {
	}
}
