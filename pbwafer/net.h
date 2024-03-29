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

#ifndef __NET_H
#define __NET_H

#ifdef _WIN32
#include <Winsock2.h>
#else
#include <netdb.h>
#include <sys/time.h>
#endif

#include "buf.h"
#include "list.h"

//TODO: UDP Packet reliability:
//          - Unreliable
//          - Unreliable Sequenced  # May only need this one for unreliable
//          - Reliable
//          - Reliable   Sequenced  # May not need this one
//          - Reliable   Ordered    # May not need this one

//TODO: Change the socket window size to an extremely large value so it can buffer more data
//      this will hopefully decrease the likelyhood of the applicatoin as being
//      the network bottleneck

//TODO: May need a circular buffer for doing partial recvs
//      and a function for handling partial sends

//TODO: Test and actually make sure that both IPv4 and IPv6 both work with all
//      network functions

//TODO: Determine if the ack bitfield needs to be 4 or 2 bytes

#define SIM_PL  1  // Packet Loss
#define SIM_MDR 2  // Maximum Data Rate
#define SIM_RTT 4  // Round Trip Time
#define SIM_DC  8  // Disconnect

#define RELI_US  0 // Unreliable Sequenced
#define RELI_R   1 // Reliable

#ifdef _WIN32
typedef int socklen_t;
#endif

// Network Simulation Variables, each hostinfo_t contains one
typedef struct netsim_s
{
	char state;        // Turn on/off network simulator
	unsigned char opt; // Which conditions should be simulated
	int pl;            // Packet Loss
	int mdr;           // Maximum Data Rate
	int rtt;           // Round Trip Time
	char dc;           // Disconnect
} netsim_t;

// Data needed to send a recieve UDP packets over a socket
typedef struct net_s
{
	int sockfd;
	struct sockaddr_storage addr;
	socklen_t addrlen;
	//size_t addrlen;

	struct timeval to; // Socket timeout, will result in a disconnect if it reaches 0

	netsim_t ns;
} net_t;

int NET_init();

// Create and send to/from sockets
net_t *NET_socket_server(const char *address, const char *service);
net_t *NET_socket_client(const char *address, const char *service);
int NET_send(net_t *n, fixedbuf_t *b);
int NET_send_reliable(net_t *n, fixedbuf_t *b);
int NET_recv(net_t *n, fixedbuf_t *b);
void NET_free(net_t *n);

// Functions to read and compare net_t
void NET_copy(net_t *dest, net_t *src);
int NET_ipcmp(net_t *n1, net_t *n2);
int NET_portcmp(net_t *n1, net_t *n2);
void NET_print(net_t *n);
char *NET_get_ip(net_t *n);
unsigned short NET_get_port(net_t *n);

// Simulate a few conditions of a real network
void NET_sim(net_t *n);
void NET_sim_state(net_t *n, char state, unsigned char opt);

#endif /* !__NET_H */
