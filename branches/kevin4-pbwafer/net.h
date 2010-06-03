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
//          - Unreliable Sequenced  # May only need this one for unreliable
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

//TODO: Add a way to get the port number and ip in a human readable form from
//      any incoming packet.  Also need a way to compare ip/port's.

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

#define RELI_US  0 // Unreliable Sequenced
#define RELI_R   1 // Reliable

typedef struct net_s
{
	//TODO: Make sure these are the only vars needed
	int sockfd;
	struct sockaddr_storage addr;
	size_t addrlen;

	//TODO: Add a queue of un-acked packets that where sent to this host

	unsigned short seq_num;

	struct timeval tv;         // Time till disconnected or RTT

	// Network Simulation Variables
	char state;        // Turn on/off network simulator
	unsigned char opt; // Which conditions should be simulated
	int pl;            // Packet Loss
	int mdr;           // Maximum Data Rate
	int mtu;           // Maximum Transmission Unit
	int rtt;           // Round Trip Time
	char dc;           // Disconnect
} net_t;

extern fixedbuf_t g_buf;
extern int g_sockfd;
extern struct sockaddr_storage g_addr;
extern size_t g_addrlen;

void NET_init();

// Create and send to/from sockets
net_t *NET_socket_server(const char *address, const char *service);
net_t *NET_socket_client(const char *address, const char *service);
int NET_send(net_t *n, fixedbuf_t *b);
int NET_send_reliable(net_t *n, fixedbuf_t *b);
int NET_recv();
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
