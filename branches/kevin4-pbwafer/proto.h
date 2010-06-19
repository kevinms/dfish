#ifndef __PROTO_H
#define __PROTO_H

//TODO: Be able to conect to a server through both LAN Broadcast and Known IP
//      Will need to maintain a list of currently connected users.  There will
//      need to be a way to distinguish between multiple clients behind NATs.
//      This will be done through Client IDs and Port #s I believe.

//      Maybe create a signature for each client that connects and they will
//      send that signature to the server in each packet.  This way malicious
//      users cannot easily spoof packets from other clients. Unless they can
//      capture another clients packets to determinee the correct ID.  Or brute
//      force I guess.  Although I should be able to detect a bruteforce attack.

// PROTO Packet Header
// +--------+--------+------+------+
// |  UDP   |  NET   | Type | Data |
// | Header | Header |      |      |
// +--------+--------+------+------+
//     8        11      1       n
// 20(!16) bytes of header data including UDP and NET headers

//TODO: Define what the connection process will be:
//          - 2 Way handshake
//          - Client: Send version #(short), username(String)
//          - Server: Send new client id(short) or error # Maybe

//TODO: Generate a random unique client ID

//TODO: Client blocking based off of:
//          - IP
//          - IP Range
//          - User Name (offensive aliases)

#include "list.h"
#include "net.h"

#define PTYPE_INFO      0
#define PTYPE_CONNECT   1
#define PTYPE_KEEPALIVE 2
#define PTYPE_ERROR     3
#define PTYPE_MSG       4
#define PTYPE_BP        5
#define PTYPE_BPDIFF    6
#define PTYPE_NAME      7

#define ERR_CONN_VER  0
#define ERR_CONN_FULL 1

#define STATE_NOP 0
#define STATE_CON 1

typedef struct header_s
{
	unsigned short size;
	unsigned short seq_num;
	unsigned char hid;
	unsigned short ack;
	unsigned long ack_bits;
} header_t;

typedef struct packet_s
{
	unsigned short seq_num;
	fixedbuf_t *b;
} packet_t;

typedef struct hostinfo_s
{
	net_t *n;
	header_t hdr;

	//TODO: Add a list (hashmap?) of un-acked packets that where sent to this host
	list_t *unacked_reliable_packets;
	// Maybe use this when implementing flow control
	list_t *queue_of_packets_to_send;
	//TODO: now in header_t
	unsigned short seq_num;

} hostinfo_t;

// A server will send this information to a client if asked for (usually before
// the client has connected to any server)
typedef struct servinfo_s
{
	const char *name;
	char major_version;
	char minor_version;
	unsigned short max_clients;
	unsigned short num_clients;
	const char **clients;  //TODO: turn this into some sort of list or retrieve
	                       //      it from the clientinfo list
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

void PROTO_init();
hostinfo_t *PROTO_host_init();

hostinfo_t *PROTO_socket_client(const char *node, const char *service);
hostinfo_t *PROTO_socket_server(const char *node, const char *service);

void PROTO_req_servinfo_ip(const char *node, const char *service);
list_t *PROTO_req_servinfo_broadcast(); // Discover servers on a LAN
list_t *PROTO_req_servinfo_master(const char *node, const char *service);

void PROTO_set_servinfo(const char *name, unsigned short max_clients);
void PROTO_set_clientinfo(int state, char *name);

hostinfo_t *PROTO_connect(const char *node, const char *service);
void PROTO_connect_ip(int count, const char **s);

void PROTO_req_name(int n, const char **s);

// Work horses of parsing functions
void PROTO_server_parse_DGRAM();
void PROTO_client_parse_DGRAM();

// Send/recv stuff
int PROTO_send_reliable(hostinfo_t *h, fixedbuf_t *b);
int PROTO_send_unreliable(hostinfo_t *h, fixedbuf_t *b);
int PROTO_send(hostinfo_t *h, fixedbuf_t *b);
int PROTO_recv();

//void PROTO_send_chat(hostinfo_t *h, const char *s);
void PROTO_client_send_chat(const char *s);
void PROTO_server_send_chat(clientinfo_t *c, const char *s);
void PROTO_change_name(const char *s);

char PROTO_is_known_host(hostinfo_t *h);
void PROTO_accept_acks();

#endif /* !__PROTO_H */
