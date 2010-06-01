#ifndef __PROTO_H
#define __PROTO_H

//TODO: Be able to conect to a server through both LAN Broadcast and Known IP
//      Will need to maintain a list of currently connected users.  There will
//      need to be a way to distinguish between multiple clients behind NATs.

//      Maybe create a signature for each client that connects and they will
//      send that signature to the server in each packet.  This way malicious
//      users cannot easily spoof packets from other clients.

// PROTO Packet Header
// +--------+--------+------+--------+------+
// |  UDP   |  NET   | Type | Client | Data |
// | Header | Header |      |   ID   |      |
// +--------+--------+------+--------+------+
//     8        5       1       2       n
// 16 bytes of header data including UDP header

//TODO: Define what the connection process will be:
//          - 3 Way handshake?
//          - Client: Send what version of dfish/pbwafer is being used
//          - Server: Send back a new client number or error # Maybe
//          - Client: Send user name

void PROTO_connect_lan();
void PROTO_connect_ip(const char *node, const char *service);

#endif /* !__PROTO_H */
