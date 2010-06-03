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
// +--------+--------+------+--------+------+
// |  UDP   |  NET   | Type | Client | Data |
// | Header | Header |      |   ID   |      |
// +--------+--------+------+--------+------+
//     8        5       1       2       n
// 16 bytes of header data including UDP and NET headers

//TODO: Define what the connection process will be:
//          - 2 Way handshake
//          - Client: Send version #(short), username(String)
//          - Server: Send new client id(short) or error # Maybe

//TODO: Generate a random unique client ID

//TODO: Client blocking based off of:
//          - IP
//          - IP Range

#include "list.h"
#include "net.h"

#define PTYPE_INFO      0
#define PTYPE_CONNECT   1
#define PTYPE_KEEPALIVE 2
#define PTYPE_ERROR     3
#define PTYPE_MSG       4
#define PTYPE_BP        5
#define PTYPE_BPDIFF    6

#define STATE_NOP 0
#define STATE_CON 1

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
	unsigned short id;
	char *name;

	//TODO: Maintain a queue of un acked packets.  This is done to delta
	//      compress position packets.
} clientinfo_t;

void PROTO_init();

void PROTO_req_servinfo_ip(const char *node, const char *service);
list_t *PROTO_req_servinfo_broadcast(); // Discover servers on a LAN
list_t *PROTO_req_servinfo_master(const char *node, const char *service);
void PROTO_send_servinfo(const char *node, const char *service, servinfo_t *info);

void PROTO_set_servinfo(const char *name, unsigned short max_clients);
void PROTO_set_clientinfo(int state, char *name);

net_t *PROTO_connect(const char *node, const char *service);
net_t *PROTO_accept_client();

// Work horses of parsing functions
void PROTO_server_parse_DGRAM();
void PROTO_client_parse_DGRAM();

#endif /* !__PROTO_H */
