#ifndef __PROTO_H
#define __PROTO_H

// PROTO Packet Header
// +--------+---------+------+--------+----------+------+------+
// | Packet | seq_num | Host |  ack   |   ack    | Type | Data |
// |  Size  |         |  ID  |        | bitfield |      |      |
// +--------+---------+------+--------+----------+------+------+
//     2         2        1      2         4        n
// 11 bytes of header data ( +8 for UDP header)

// To understand some of this code figure out who you are.  Are you the client
// or the server?  Whoever you are, your buddy is the opposite.
// h->hdr.seq_num  : Your seq_num
// h->hdr.ack      : Should be the last seq_num you saw from your buddy (we are updating this)
// g_h.hdr.seq_num : Buddies seq_num
// g_h.hdr.ack     : The seq_num of the last packet your buddy saw

//TODO: Generate a random unique client ID

//TODO: Client blocking based off of:
//          - IP
//          - IP Range
//          - User Name (offensive aliases)

#include "list.h"
#include "net.h"
#include "timer.h"

#define PTYPE_INFO      0
#define PTYPE_CONNECT   1
#define PTYPE_DC        2
#define PTYPE_KEEPALIVE 3
#define PTYPE_ERROR     4
#define PTYPE_MSG       5
#define PTYPE_BP        6
#define PTYPE_BPDIFF    7
#define PTYPE_NAME      8

#define ERR_CONN_VER  0
#define ERR_CONN_FULL 1

#define STATE_NOP          0
#define STATE_WAIT_ON_INFO 1
#define STATE_WAIT_ON_CON  2
#define STATE_CON          3

#define TIMEOUT   1000
#define DROP_TIME 1500

#define NUMBITS 32
#define MAX_SHORT 65535

// Holds all header info
typedef struct header_s
{
	unsigned short size;
	unsigned short seq_num;
	unsigned char hid;
	unsigned short ack;
	unsigned long ack_bits;
} header_t;

// Used to store a copy of a packet to possibly resend later
typedef struct packet_s
{
	unsigned short seq_num;
	fixedbuf_t *b;
	pwtimer_t timer;
} packet_t;

// Every host will store its info in this, server and client
typedef struct hostinfo_s
{
	net_t *n;
	header_t hdr;

	pwtimer_t keepalive;

	packet_t *unacked_reliable_packet_map[NUMBITS];
	unsigned short latest_ack_seen;

	list_t *packet_queue; // Maybe use this when implementing flow control
} hostinfo_t;

// A server will send this information to a client if asked for (usually before
// the client has connected to any server)
typedef struct servinfo_s
{
	const char *ip;
	unsigned short port;
	const char *name;
	char major_version;
	char minor_version;
	unsigned short max_clients;
	unsigned short num_clients;
	const char **clients;
	unsigned short ping;
} servinfo_t;

// Data pertaining to each individual client.  Created once a client sends
// a connection request (if there are enough player slots).
typedef struct clientinfo_s
{
	int state;
	char *name;

	hostinfo_t *info;
} clientinfo_t;

extern fixedbuf_t g_buf;
extern hostinfo_t g_h;

// Used by client and server
void PROTO_init();
hostinfo_t *PROTO_host_init();
int PROTO_send_reliable(hostinfo_t *h, fixedbuf_t *b);
int PROTO_send_unreliable(hostinfo_t *h, fixedbuf_t *b);
int PROTO_send(hostinfo_t *h, fixedbuf_t *b);
int PROTO_recv();
char PROTO_is_known_host(hostinfo_t *h);
void PROTO_update_acks(hostinfo_t *h);
void PROTO_accept_acks(hostinfo_t *h);
void PROTO_handle_timeout(hostinfo_t *h);
void PROTO_queue_packet(hostinfo_t *h, packet_t *p);
void PROTO_print_header_t(header_t *h);
void PROTO_print_header(unsigned char *buf);

// Handles sequence number and ack wrap-around
int PROTO_more_recent(unsigned short s1, unsigned short s2, unsigned short max_sequence);
unsigned short PROTO_diff(unsigned short s1, unsigned short s2, unsigned short max_sequence);

// Used by client
void PROTO_set_clientinfo(int state, char *name);
void PROTO_client_netsim(char state, unsigned char opt);
hostinfo_t *PROTO_socket_client(const char *node, const char *service);
hostinfo_t *PROTO_connect(const char *node, const char *service);
void PROTO_connect_ip(int count, const char **s);
void PROTO_disconnect();
void PROTO_req_servinfo_ip(const char *node, const char *service);
void PROTO_req_servinfo_broadcast(); // Discover servers on a LAN
list_t *PROTO_req_servinfo_master(const char *node, const char *service);
void PROTO_req_name(int n, const char **s);
void PROTO_client_parse_DGRAM();
void PROTO_client_send_chat(const char *s);
void PROTO_server_send_chat(clientinfo_t *c, const char *s);
void PROTO_change_name(const char *s);
void PROTO_client_send_packets();
void PROTO_client_keepalive();

// Used by server
void PROTO_set_servinfo(const char *name, unsigned short max_clients);
hostinfo_t *PROTO_socket_server(const char *node, const char *service);
void PROTO_server_parse_DGRAM();
void PROTO_server_send_packets();

#endif /* !__PROTO_H */
