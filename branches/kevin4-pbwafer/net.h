#ifndef __NET_H
#define __NET_H

#include <netdb.h>
#include <sys/time.h>

#include "buf.h"

//TODO: Add in some options to simulate different network conditions
//          - Max data rate
//          - Custom MTU
//          - Packet loss
//          - Propogation time variance
//          - Disconnect

//TODO: UDP Packet reliability:
//          - Unreliable
//          - Unreliable Sequenced
//          - Reliable
//          - Reliable   Sequenced  # May not need this one
//          - Reliable   Ordered    # May not need this one

//TODO: May need a circular buffer for doing partial recvs
//      and a function for handling partial sends

//TODO: Test and actually make sure that both IPv4 and IPv6 both work with all
//      network functions

//TODO: The network simulator could be implemented in serveral ways:
//          - Preprocessor Macros  # Would have to be enabled compile time (meh)
//          - If statements in NET_send() and NET_recv()
//          - Some kind of state machine

// NET Packet Header
// +--------+--------+-------------+------------+------+
// |  UDP   | Packet |   Packet    | Sequence # | Data |
// | Header |  Size  | Reliability | (Optional) |      |
// +--------+--------+-------------+------------+------+
//     8        2           1            2         n

#define SIM_PL  1  // Packet Loss
#define SIM_MDR 2  // Maximum Data Rate
#define SIM_MTU 4  // Maximum Transmission Unit
#define SIM_RTT 8  // Round Trip Time
#define SIM_DC  16 // Disconnect

typedef struct net_s
{
	int sockfd;                // Socket file descriptor
	const char *node;          // IP address of server
	const char *service;       // Port Number (string)
	unsigned short port;       // Port number

/*
	struct addrinfo hints;     // Address info IP version agnostic
	struct addrinfo *servinfo; // Filled after a getaddrinfo()
	struct addrinfo *p;
*/

	//TODO: Make sure these are thee only structs needed
	struct sockaddr *addr;
	size_t addrlen;

	struct timeval tv;         // 

	// Network Simulation Variables
	int state;
	unsigned char opt;
} net_t;

// Create and send to/from sockets
net_t *NET_socket_server(const char *address, const char *service);
net_t *NET_socket_client(const char *address, const char *service);
int NET_send(net_t *n, fixedbuf_t *b);
int NET_recv(net_t *n, fixedbuf_t *b);
void NET_free(net_t *n);

// Simulate a few conditions of a real network
void NET_sim(net_t *n);
void NET_sim_state(net_t *n, char state);
void NET_sim_pl(net_t *n);
void NET_sim_mdr(net_t *n);
void NET_sim_mtu(net_t *n);
void NET_sim_rtt(net_t *n);
void NET_sim_dc(net_t *n);

#endif /* !__NET_H */
