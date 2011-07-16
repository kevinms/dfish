// Copyright (C) 2011  Kevin Smith
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#include "net.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include "console.h"
#include "utils.h"

#ifdef _WIN32
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

//TODO:URGENT: Get a basic flow control algorithm working based of a few metrics

// Initilizes the NET module, called by PBWAFER_init()
int NET_init()
{
#ifdef _WIN32
	WSADATA WsaData;
	if (WSAStartup( MAKEWORD(2,2), &WsaData ) == NO_ERROR)
		return 1;

	fprintf(stderr, "ERROR: could not initialize network module\n");
	return 0;
#else
	return 1;
#endif
}

// Create a socket for the server to communicate on and fill a net_t
net_t *NET_socket_server(const char *node, const char *service)
{
	net_t *n;
	int rv;
	int yes = 1;
	unsigned long ulMode = 1;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct addrinfo *p;

	n = (net_t *)malloc(sizeof(*n));

	// Fill the hints struct with IP version agnostic constants and UDP socktype
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // Says that it will be a server

	// Get a listing of interfaces we may be able to bind to
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

		// Lets us bind to a socket even if it appears to be in use by another program
		if (setsockopt(n->sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			return NULL;
		}

		if (bind(n->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
#ifdef _WIN32
			closesocket(n->sockfd);
#else
			close(n->sockfd);
#endif
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "Error: server failed to bind socket\n");
		return NULL;
	}

	// Set the socket to non-blocking
#ifdef WIN32
	ioctlsocket( n->sockfd, FIONBIO, &ulMode );
#else
	fcntl(n->sockfd, F_SETFL, fcntl(n->sockfd, F_GETFL)|O_NONBLOCK);
#endif
	

	// Store the sockaddr and length in the net_t
	memcpy(&n->addr, p->ai_addr, p->ai_addrlen);
	n->addrlen = p->ai_addrlen;

	// Set the initial state of the network simulator to off
	n->ns.state = 0;

	// Can go ahead and free servinfo since it is not stored in net_t anymore
	freeaddrinfo(servinfo);

	return n;
}

// Create a socket for the server to communicate on and fill a net_t
net_t *NET_socket_client(const char *node, const char *service)
{
	net_t *n;
	int rv;
	unsigned long ulMode = 1;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct addrinfo *p;

	n = (net_t *)malloc(sizeof(*n));

	// Fill the hints struct with IP version agnostic constants and UDP socktype
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	// Get a listing of interfaces we may be able to make a socket on
	if ((rv = getaddrinfo(node, service, &hints, &servinfo)) != 0) {
		fprintf(stdout, "getaddrinfo error: %s\n", gai_strerror(rv));
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
		fprintf(stderr, "Error: client failed to bind socket\n");
		return NULL;
	}

	// Set the socket to non-blocking
#ifdef WIN32
	ioctlsocket(n->sockfd, FIONBIO, &ulMode);
#else
	fcntl(n->sockfd, F_SETFL, fcntl(n->sockfd, F_GETFL)|O_NONBLOCK);
#endif

	// Store the sockaddr and length in the net_t
	memcpy(&n->addr, p->ai_addr, p->ai_addrlen);
	n->addrlen = p->ai_addrlen;

	// Set the initial state of the network simulator to off
	n->ns.state = 0;

	// Can go ahead and free servinfo since it is not stored in net_t anymore
	freeaddrinfo(servinfo);

	return n;
}

//TODO: Handle partial sends
int NET_send(net_t *n, fixedbuf_t *b)
{
	int numbytes;

	// Simulate a few network conditions
	NET_sim(n);
	if(n->ns.state) {
		if(n->ns.dc == 1) {
			printf("disconnecting (pretend)\n");
			return b->cursize;
		}
		if(n->ns.pl > 0) {
			printf("numbytes sent(pretend): %d\n",b->cursize);
			return b->cursize;
		}
	}

	if ((numbytes = sendto(n->sockfd, b->buf, b->cursize, 0, (struct sockaddr *)&n->addr, n->addrlen)) == -1) {
		perror("Error: sendto failed");
	}

	//if(numbytes > 0)
	//	printf("\tnumbytes sent: %d\n",numbytes);

	return numbytes;
}

//TODO: Handle partial recvs
//TODO: Do we want a nonblocking socket or or nonblocking recv?
int NET_recv(net_t *n, fixedbuf_t *b)
{
	int numbytes;

	buf_clear(b);
	if ((numbytes = recvfrom(n->sockfd, b->buf, b->maxsize , 0, (struct sockaddr *)&n->addr, &n->addrlen)) == -1) {
		//perror("recvfrom");
	}

	if(numbytes <= 0)
		return numbytes;


	//printf("numbytes recved: %d\n",numbytes);
	buf_clear(b);
	b->cursize = numbytes;

	return numbytes;
}

// Creates a copy of src and puts it in dest, dest needs to be allocated memory
// outside of this function
void NET_copy(net_t *dest, net_t *src)
{
	dest->sockfd = src->sockfd;
	memcpy((void *)&(dest->addr), (void *)&src->addr, src->addrlen);
	dest->addrlen = src->addrlen;
}

// Compare the ip's of two net_t structs
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

// Compare the port's of two net_t structs
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

//TODO: why does this not use NET_get_ip and NET_get_port
// Print the IP address and Port number to stdout
void NET_print(net_t *n)
{
	char *ip = NULL;
	unsigned short port;

	ip = NET_get_ip(n);
	port = NET_get_port(n);

	if(ip) {
		printf("IP: %s:%d\n", ip, port);
		free(ip);
	} else {
		printf("IP: ERROR\n");
	}

	/*
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
	*/
}

// Gets a human readable string representation of the IP
char *NET_get_ip(net_t *n)
{
	char ip[INET6_ADDRSTRLEN];

#ifdef _WIN32
	if(!getnameinfo((struct sockaddr *)&n->addr, n->addrlen, ip, INET6_ADDRSTRLEN, NULL, 0, 0))
		return strdup(ip);
	printf("ERROR: cannot get ip\n");
	return NULL;
#else
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
#endif
}

// Gets a human readable unsigned short of the PORT
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

// Free all memory in a net_t
void NET_free(net_t *n)
{
	
}

// Simulate certain network conditions
void NET_sim(net_t *n)
{
	int i = 0;

	// If the netsim is turned off return
	if(!n->ns.state)
		return;

	// Depending on what bits are set in the opt bitfield run different simulationss
	if(n->ns.opt & SIM_DC) {
		// Supposedly 1 in 1000 chance of disconnecting
		if(rand_max(1000) == 2)
			n->ns.dc = 1;
		//CONSOLE_print("dc = %d", n->ns.dc);
	}
	if(n->ns.opt & SIM_PL) {
		// Since packet loss is usually more than one packet when it happens
		// this will result in a series of lost packets
		if(n->ns.pl > 0)
			n->ns.pl--;
		else if((i = rand_max(100)) < 10)
			n->ns.pl = i;
		//CONSOLE_print("pl = %d", n->ns.pl);
	}
}

// Set the state and options bitfield for a network simulation
void NET_sim_state(net_t *n, char state, unsigned char opt)
{
	n->ns.state = state;
	n->ns.opt  = opt;

	if(state) {
		srand(time(NULL));
		n->ns.dc = 0;
		n->ns.pl = 0;
	}
}
