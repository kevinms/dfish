#ifndef __NET_H
#define __NET_H

#include <netdb.h>
#include <sys/time.h>

#include "buf.h"
#include "list.h"

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
// +--------+---------+------+--------+----------+------+
// | Packet | seq_num | Host |  ack   |   ack    | Data |
// |  Size  |         |  ID  | offset | bitfield |      |
// +--------+---------+------+--------+----------+------+
//     2         2        1      2         4        n

//TODO: Determine if the ack bitfield needs to be 4 or 2 bytes

#define SIM_PL  1  // Packet Loss
#define SIM_MDR 2  // Maximum Data Rate
#define SIM_MTU 4  // Maximum Transmission Unit
#define SIM_RTT 8  // Round Trip Time
#define SIM_DC  16 // Disconnect

#define RELI_US  0 // Unreliable Sequenced
#define RELI_R   1 // Reliable

typedef struct netsim_s
{
	// Network Simulation Variables
	char state;        // Turn on/off network simulator
	unsigned char opt; // Which conditions should be simulated
	int pl;            // Packet Loss
	int mdr;           // Maximum Data Rate
	int mtu;           // Maximum Transmission Unit
	int rtt;           // Round Trip Time
	char dc;           // Disconnect
} netsim_t;

typedef struct net_s
{
	//TODO: Make sure these are the only vars needed
	int sockfd;
	struct sockaddr_storage addr;
	size_t addrlen;

	struct timeval to; // 
	struct timeval tv; // Time till disconnected or RTT

	netsim_t ns;
} net_t;

void NET_init();

// Create and send to/from sockets
net_t *NET_socket_server(const char *address, const char *service);
net_t *NET_socket_client(const char *address, const char *service);
int NET_send(net_t *n, fixedbuf_t *b);
int NET_send_reliable(net_t *n, fixedbuf_t *b);
int NET_recv(net_t *n, fixedbuf_t *b);
void NET_copy(net_t *dest, net_t *src);

int NET_ipcmp(net_t *n1, net_t *n2);
int NET_portcmp(net_t *n1, net_t *n2);
void NET_print(net_t *n);
char *NET_get_ip(net_t *n);
unsigned short NET_get_port(net_t *n);

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
