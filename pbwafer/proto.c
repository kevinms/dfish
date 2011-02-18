#include "proto.h"

#include <assert.h>

#include "string.h"
#include "stdio.h"
#include "utils.h"
#include "version.h"
#include "console.h"

list_t *servinfo_list;   // This list is filled when a client does a PROTO_req_servinfo()
list_t *clientinfo_list; // This list contains clientinfo for each client connected to a server

servinfo_t servinfo;     // Filled by the server with information about itself
clientinfo_t clientinfo; // Filled by the client with informaiton about itself, including the current open socket

fixedbuf_t g_buf; // Global recieve buffer, used by PROTO_recv()
hostinfo_t g_h;   // Each time a PROTO_recv() is called this is filled with information about host

// Initialize the PROTO module
void PROTO_init()
{
	servinfo_list = list_init();   // Init the list of servinfo_t's the client will use
	clientinfo_list = list_init(); // Init the list of clients the server will use
	clientinfo.state = STATE_NOP;  // Set the client state initially to disconnected

	// Initialize the global recieve buffer
	buf_init(&g_buf,512);
	g_h.n = (net_t *)malloc(sizeof(*g_h.n));
	g_h.n->sockfd = 0;
	g_h.n->addrlen = sizeof(g_h.n->addr);
}

// Initialize a new hostinfo_t
hostinfo_t *PROTO_host_init()
{
	hostinfo_t *h;
	int i;

	h = (hostinfo_t *)malloc(sizeof(*h));
	for(i = 0; i < NUMBITS; i++)
		h->unacked_reliable_packet_map[i] = NULL;
	h->packet_queue = list_init();

	// Set the the whole header to 0 (including h->hdr.seq_num)
	memset((void *)&h->hdr, 0, sizeof(h->hdr));
	h->hdr.seq_num = -1;

	return h;
}

// Create a new hostinfo_t and copy the net_t from src
hostinfo_t *PROTO_host_copy(hostinfo_t *src)
{
	hostinfo_t *h;

	h = PROTO_host_init();

	h->n = (net_t *)malloc(sizeof(*h->n));
	NET_copy(h->n, g_h.n);

	return h;
}

// Callback for the console /netsim command
// Used by client turn on and off the network simulator
void PROTO_client_netsim(char state, unsigned char opt)
{
	// Make sure we have a socket before before trying to change the state of the netsim
	if(clientinfo.info != NULL)
		if(clientinfo.info->n != NULL) {
			NET_sim_state(clientinfo.info->n, state, SIM_DC|SIM_PL);
			CONSOLE_print("netsim [%d]", state);
		}
}

// Create a new socket for the client to communicate on
hostinfo_t *PROTO_socket_client(const char *node, const char *service)
{
	hostinfo_t *h;

	// Initialize a new hostinfo and net_t
	h = PROTO_host_init();
	h->n = NET_socket_client(node, service);
	if(h->n == NULL)
		return NULL;  //TODO: host is never deallocated (memory leak)

	// Set the global recieve socket to the newly created socket
	g_h.n->sockfd = h->n->sockfd;

	return h;
}

// Create a new socket for the server to communicate on
hostinfo_t *PROTO_socket_server(const char *node, const char *service)
{
	hostinfo_t *h;

	// Initialize a new hostinfo and net_t
	h = PROTO_host_init();
	h->n = NET_socket_server(node, service);
	if(h->n == NULL)
		return NULL;  //TODO: host is never deallocated (memory leak)

	// Set the global recieve socket to the newly created socket
	g_h.n->sockfd = h->n->sockfd;

	return h;
}

// Used by the client to request server info from a specific server
void PROTO_req_servinfo_ip(const char *node, const char *service)
{
	hostinfo_t *h;
	fixedbuf_t *b;

	// Create a new socket for the client to communicate on
	h = PROTO_socket_client(node, service);
	if(!h) {
		CONSOLE_print("Error: Could not create client socket");
		return;
	}

	// Setup header
	b = buf_new_init(512);
	buf_memget(b, 11);
	buf_write_char(b, PTYPE_INFO);

	clientinfo.state = STATE_WAIT_ON_INFO;

	PROTO_send_reliable(h, b);
}

// Used by the client to request server info from all servers running on this
// service on its local LAN (Will disconnect the client from any server!)
void PROTO_req_servinfo_broadcast(const char *service)
{
	hostinfo_t *h;
	fixedbuf_t *b;
	int broadcast = 1;

	// Create a new socket for the client to communicate on
	h = PROTO_socket_client("255.255.255.255", service);
	if(!h) {
		CONSOLE_print("Error: Could not create client socket");
		return;
	}

	// Set sock opt SO_BROADCAST so we can use the broadcast addresse
	if (setsockopt(h->n->sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
		perror("setsockopt (SO_BROADCAST)");
		return;
	}

	// Write packet
	b = buf_new_init(512);
	buf_memget(b, 11);
	buf_write_char(b, PTYPE_INFO);

	clientinfo.state = STATE_WAIT_ON_INFO;

	PROTO_send_reliable(h, b);
}

// Used by the server to set the global servinfo
void PROTO_set_servinfo(const char *name, unsigned short max_clients)
{
	servinfo.name = strdup(name);
	servinfo.max_clients = max_clients;
}

// Used by the client to set the global clientinfo
void PROTO_set_clientinfo(int state, char *name)
{
	clientinfo.state = state;
	clientinfo.name = strdup(name);
}

// Used by client to connect to a server by ip and port
hostinfo_t *PROTO_connect(const char *node, const char *service)
{
	fixedbuf_t *b;

	// If the client is currently connected to a server, disconnect from it
	if(clientinfo.state == STATE_CON)
		PROTO_disconnect();

	// TODO: May only need to change the old socket instead of creating a new one
	//       after all udp sockets are connectionless
	// Create a new socket for the client to communicate on
	clientinfo.info = PROTO_socket_client(node, service);
	if(!clientinfo.info) {
		CONSOLE_print("Error: Could not create client socket");
		return NULL;
	}

	// Write packet
	b = buf_new_init(14 + strlen(clientinfo.name));
	buf_memget(b, 11);
	buf_write_char(b, PTYPE_CONNECT);
	buf_write_char(b, MAJOR_VERSION);
	buf_write_char(b, MINOR_VERSION);
	buf_write_string(b, clientinfo.name);

	clientinfo.state = STATE_WAIT_ON_CON;

	PROTO_send_reliable(clientinfo.info, b);

	CONSOLE_print("Connecting...");

	return clientinfo.info;
}

// Callback for the console /connect command
void PROTO_connect_ip(int count, const char **s)
{
	PROTO_connect(s[0],s[1]);
}

// Used by client to tell the server it is disconnecting
void PROTO_disconnect()
{
	fixedbuf_t *b;

	// If the client is not connected to a server stop
	if(clientinfo.state == STATE_NOP) {
		CONSOLE_print("Error: not connected to a server");
		return;
	}

	// Write packet
	b = buf_new_init(512);
	buf_memget(b, 11);
	buf_write_char(b, PTYPE_DC);

	PROTO_send_reliable(clientinfo.info, b);

	// Now that it send the disconnect packet, set the client to the disconnected state
	clientinfo.state = STATE_NOP;
}

// Callback for the console /name command
// Used by client to send a request for a name change to the server
void PROTO_req_name(int n, const char **s)
{
	fixedbuf_t *b;

	// If the client is not connected to a server stop
	if(clientinfo.state == STATE_NOP) {
		CONSOLE_print("Error: not connected to a server");
		return;
	}

	// Write packet
	b = buf_new_init(512);
	buf_memget(b, 11);
	buf_write_char(b, PTYPE_NAME);
	buf_write_string(b, s[0]);

	PROTO_send_reliable(clientinfo.info, b);
}

// This function will parse packets sent to the server
void PROTO_server_parse_DGRAM()
{
	int type;
	link_t *tmp;
	clientinfo_t *c;
	fixedbuf_t *b;
	char *s;

	//buf_init(&b, 512);
	b = buf_new_init(512);

	// Read the packet type from the global recieve buffer
	type = buf_read_char(&g_buf);

	// Host is requeseting servinfo
	if(type == PTYPE_INFO) {
		printf("PTYPE_INFO\n");

		// Setup header
		buf_memget(b, 11);
		buf_write_char(b, PTYPE_INFO);

		// Write packet data
		buf_write_string(b, servinfo.name);
		buf_write_char(b, MAJOR_VERSION);
		buf_write_char(b, MINOR_VERSION);
		buf_write_short(b, servinfo.max_clients);

		// Write all client names currently connected to the server
		buf_write_short(b, clientinfo_list->len);
		for(tmp = clientinfo_list->head; tmp != NULL; tmp = tmp->next)
			buf_write_string(b, ((clientinfo_t *)tmp->item)->name);

		//TODO: check rv to make sure there were no errors
		PROTO_send(&g_h, b);
	}

	// Host is requeseting to connect
	else if(type == PTYPE_CONNECT) {

		// Check if there are any slots available on the server
		if(clientinfo_list->len == servinfo.max_clients) {
			buf_memget(b, 11);
			buf_write_char(b, PTYPE_ERROR);
			buf_write_char(b, ERR_CONN_FULL);

			PROTO_send(&g_h, b);
		}
		// Check if the client is using the same game version
		else if(buf_read_char(&g_buf) == MAJOR_VERSION && buf_read_char(&g_buf) == MINOR_VERSION) {
			printf("Same version!\n");

			// Create and initialize a new clientinfo_t for the new client
			c = (clientinfo_t *)malloc(sizeof(*c));
			c->name = buf_read_string(&g_buf);
			c->info = PROTO_host_init();
			c->info->n = (net_t *)malloc(sizeof(*c->info->n));
			NET_copy(c->info->n, g_h.n);
			c->info->hdr.hid = 10; //TODO: Generate a host id and move this to the payload

			// Add the new client to the servers list of clients
			list_add(clientinfo_list, (void *)c);

			// Write packet
			buf_memget(b, 11);
			buf_write_char(b, PTYPE_CONNECT);
			buf_write_char(b, c->info->hdr.hid); //TODO: Generate a host id

			// Manage reciever and sender acks
			PROTO_update_acks(c->info);
			PROTO_accept_acks(c->info);

			// Reset the keepalive timer
			TIMER_init(&c->info->keepalive,TIMEOUT);

			//TODO: check rv to make sure there were no errors
			PROTO_send_reliable(c->info, b);
		} else {
			// Write error packet, client has a different version
			buf_memget(b, 11);
			buf_write_char(b, PTYPE_ERROR);
			buf_write_char(b, ERR_CONN_VER);
			buf_write_char(b, MAJOR_VERSION);
			buf_write_char(b, MINOR_VERSION);

			PROTO_send(&g_h, b);
		}
		return;
	}

	// Check if the packet is from a known host
	for(tmp = clientinfo_list->head; tmp != NULL; tmp = tmp->next)
		if(PROTO_is_known_host(((clientinfo_t *)tmp->item)->info))
			break;

	// If it was not then return
	if(tmp == NULL)
		return;

	// Store a pointer to the matched host's clientinfo_t
	c = (clientinfo_t *)tmp->item;

	// Manage reciever and sender acks
	PROTO_update_acks(c->info);
	PROTO_accept_acks(c->info);

	// Reset the keepalive timer
	TIMER_init(&c->info->keepalive,TIMEOUT);

	// Client wants to keep the connection alive
	if(type == PTYPE_KEEPALIVE) {
		//TODO: This was a reply from a server KEEPALIVE packet.  The reply
		//      tells the server that the client is still connected and we
		//      should reset the timeout for the client.
		buf_memget(b, 11);
		buf_write_char(b, PTYPE_KEEPALIVE);

		PROTO_send_reliable(c->info, b);
	}
	// Client sent a chat message
	else if(type == PTYPE_MSG) {
		s = buf_read_string(&g_buf);
		printf("PTYPE_MSG: '%s'\n",s);
		PROTO_server_send_chat(c,s);
		if(s != NULL)
			free(s);
	}
	// Client is requesting a name change
	else if(type == PTYPE_NAME) {
		// Read and store new name
		c->name = buf_read_string(&g_buf);

		// Write packet
		buf_memget(b, 11);
		buf_write_char(b, PTYPE_NAME);
		buf_write_string(b, c->name);

		PROTO_send_reliable(c->info, b);
	}
	// Client is disconnecting
	else if(type == PTYPE_DC) {
		printf("PTYPE_DC\n");

		// Remove client from the list of clients
		list_del_item(clientinfo_list, (void *)c);
	}
}

// This function will parse packets sent to the client
void PROTO_client_parse_DGRAM()
{
	int i;
	char type;
	servinfo_t *info;
	char *s, *s2;

	// Read the packet type from the global recieve buffer
	type = buf_read_char(&g_buf);

	// Server sent info about itself
	if(type == PTYPE_INFO && clientinfo.state == STATE_WAIT_ON_INFO)
	{
		// Create a servinfo_t to store all the info in
		info = (servinfo_t *)malloc(sizeof(*info));

		// Fill servinfo_t
		info->ip =            NET_get_ip(g_h.n);
		info->port =          NET_get_port(g_h.n);
		info->name =          buf_read_string(&g_buf);
		info->major_version = buf_read_char(&g_buf);
		info->minor_version = buf_read_char(&g_buf);
		info->max_clients =   buf_read_short(&g_buf);

		// Read all the client names connected to the server
		info->num_clients = buf_read_short(&g_buf);
		info->clients = (const char **)malloc(sizeof(info->clients)*info->num_clients);
		for(i = 0; i < info->num_clients; i++)
			info->clients[i] = buf_read_string(&g_buf);

		//TODO: Calculate ping

		// Add the new servinfo to the servinfo list
		list_add(servinfo_list, info);

		// Print servinfo to the console
		CONSOLE_print_no_update("Server Info: ");
		CONSOLE_print_no_update("    %s:%d",info->ip,info->port);  //TODO: free the ip string
		CONSOLE_print_no_update("    name: %s", info->name);
		CONSOLE_print_no_update("    version: %d", info->major_version);
		CONSOLE_print_no_update("    version: %d", info->minor_version);
		CONSOLE_print_no_update("    max_clients: %d", info->max_clients);
		CONSOLE_print_no_update("    num_clients: %d", info->num_clients);
		CONSOLE_print_no_update("    clients: ");
		for(i = 0; i < info->num_clients; i++)
			CONSOLE_print_no_update("        %s, ", info->clients[i]);
		CONSOLE_update();

		return;
	}
	// Server accepted the connection request
	else if(type == PTYPE_CONNECT && clientinfo.state == STATE_WAIT_ON_CON) {

		// Set the client state to connected and read new HID
		clientinfo.state = STATE_CON;
		clientinfo.info->hdr.hid = buf_read_char(&g_buf);

		// Manage reciever and sender acks
		PROTO_update_acks(clientinfo.info);
		PROTO_accept_acks(clientinfo.info);

		// Reset the keepalive timer
		TIMER_init(&clientinfo.info->keepalive,TIMEOUT);

		CONSOLE_print("Connected");
		return;
	}

	// Server sent an error
	else if(type == PTYPE_ERROR) {
		printf("ERROR: ");

		// Read the error type
		type = buf_read_char(&g_buf);

		if(type == ERR_CONN_VER)
			CONSOLE_print("Can not connect: wrong game version");
		else if(type == ERR_CONN_FULL)
			CONSOLE_print("Can not connect: server full");
		else
			CONSOLE_print("Unkown error: %d, send in a bugreport at code.google.com/p/dfish :)", type);

		return;
	}

	// Check if the packet is from a known host
	if(!PROTO_is_known_host(clientinfo.info))
		return;

	// Manage reciever and sender acks
	PROTO_update_acks(clientinfo.info);
	PROTO_accept_acks(clientinfo.info);

	// Reset the keepalive timer
	TIMER_init(&clientinfo.info->keepalive,TIMEOUT);


	// Server wants to keep the connection alive
	if(type == PTYPE_KEEPALIVE) {
		
	}
	// Server sent a chat message
	else if(type == PTYPE_MSG) {
		s = buf_read_string(&g_buf);
		s2 = buf_read_string(&g_buf);
		CONSOLE_print("%s: %s",s,s2);
	}
	// Server accepted name change
	else if(type == PTYPE_NAME) {
		clientinfo.name = buf_read_string(&g_buf);
		CONSOLE_print("Name changed to: %s",clientinfo.name);
	}
	// Server disconnected this client
	else if(type == PTYPE_DC) {
		// Set the client state to disconnected
		clientinfo.state = STATE_NOP;
		CONSOLE_print("Disconnected");
		//TODO: clean up allocated memory, finish the dc process
	}
}

// Sends a packet that will be resent if it appears to have been lost
int PROTO_send_reliable(hostinfo_t *h, fixedbuf_t *b)
{
	packet_t *p;
	int rv;

	rv = PROTO_send(h, b);

	// Store packet data in a packet_t
	p = (packet_t *)malloc(sizeof(*p));
	p->seq_num = h->hdr.seq_num;
	p->b = b;
	TIMER_init(&p->timer,DROP_TIME); // This timeout may depend on certain network conditions or may need to be set to some 'good' value

	// Add the new packet_t to the list of unacked_reliable_packets
	if( h->unacked_reliable_packet_map[p->seq_num%NUMBITS] != NULL)
		printf("hmmmm...\n");
	h->unacked_reliable_packet_map[p->seq_num%NUMBITS] = p;

	return rv;
}

// Sends a packet that does not matter if it is dropped
int PROTO_send_unreliable(hostinfo_t *h, fixedbuf_t *b)
{
	return PROTO_send(h, b);
	//TODO: free b!!!
}

// Try to send a single UDP packet
int PROTO_send(hostinfo_t *h, fixedbuf_t *b)
{
	int rv;

	// Increment your seq_num
	h->hdr.seq_num++;

	// Write packet header
	h->hdr.size = b->cursize;
	buf_reset(b);
	buf_write_short(b, h->hdr.size);
	buf_write_short(b, h->hdr.seq_num);
	buf_write_char(b, h->hdr.hid);
	buf_write_short(b, h->hdr.ack);
	buf_write_long(b, h->hdr.ack_bits);
	b->cursize = h->hdr.size;

	rv = NET_send(h->n, b);
	//TODO: check rv to make sure NET_send() sent the whole packet
	assert(rv == b->cursize);

	printf("SEND: seq_num[%d], ack[%d], ack_bits[",h->hdr.seq_num,h->hdr.ack);
	print_bits32(h->hdr.ack_bits);
	printf("]\n");

	// Reset the keepalive timer
	TIMER_init(&h->keepalive,TIMEOUT);

	return rv;
}

// Try to recieve a single UDP packet
int PROTO_recv()
{
	int rv;

	// If the global recieve socket is 0 then we know we are not connected to another host
	// It is set to 0 in PROTO_init()
	if(g_h.n->sockfd == 0)
		return -1;

	rv = NET_recv(g_h.n, &g_buf);

	//TODO: check rv to make sure NET_recv actually recved a packet and didn't return 0
	assert(rv != 0);

	if(rv > 0) {
		// Read the application header into g_h.hdr
		g_h.hdr.size     = buf_read_short(&g_buf);
		g_h.hdr.seq_num  = buf_read_short(&g_buf);
		g_h.hdr.hid      = buf_read_char(&g_buf);
		g_h.hdr.ack      = buf_read_short(&g_buf);
		g_h.hdr.ack_bits = buf_read_long(&g_buf);

		printf("RECV: seq_num[%d], ack[%d], ack_bits[",g_h.hdr.seq_num,g_h.hdr.ack);
		print_bits32(g_h.hdr.ack_bits);
		printf("]\n");

	}

	return rv;
}

// Used by the client to send a chat message to the server
void PROTO_client_send_chat(const char *s)
{
	fixedbuf_t *b;

	// If the client is not connected to a server then don't send a message
	if(clientinfo.state == STATE_NOP) {
		CONSOLE_print("Error: not connected to a server");
		return;
	}

	// Write packet
	b = buf_new_init(512);
	buf_memget(b, 11);
	buf_write_char(b, PTYPE_MSG);
	buf_write_string(b, s);

	PROTO_send_reliable(clientinfo.info, b);
}

// Used by the server to send a chat message recieved from a client to ALL clients
void PROTO_server_send_chat(clientinfo_t *c, const char *s)
{
	fixedbuf_t *b;
	link_t *tmp;

	// Write packet
	b = buf_new_init(512);
	buf_memget(b, 11);
	buf_write_char(b, PTYPE_MSG);
	buf_write_string(b, c->name);
	buf_write_string(b, s);

	// Send the chat message to all clients connected to the server
	for(tmp = clientinfo_list->head; tmp != NULL; tmp = tmp->next)
		PROTO_send_reliable(((clientinfo_t *)tmp->item)->info, b);  //TODO:URGENT: b needs to be copied each time, so it can be freed each time a client acks
}

// Check if the last packet was from a known host
char PROTO_is_known_host(hostinfo_t *h)
{
	// Check HID
	if(h->hdr.hid != g_h.hdr.hid) {
		printf("Error: recieved packet from unkown HID\n");
		return 0;
	}
	// Check IPs
	if(NET_ipcmp(h->n, g_h.n)) {
		printf("Error: recieved packet from unkown IP\n");
		return 0;
	}
	// Check Ports
	if(NET_portcmp(h->n, g_h.n)) {
		printf("Error: recieved packet from unkown PORT\n");
		return 0;
	}

	return 1;
}

// Take the seq_num of the latest packet you received from your buddy and update
// the ack and ack_bits to send back to your buddy
void PROTO_update_acks(hostinfo_t *h)
{
	// Store the new ack
	if(PROTO_more_recent(g_h.hdr.seq_num, h->hdr.ack, MAX_SHORT)) {
		h->hdr.ack_bits = h->hdr.ack_bits << (PROTO_diff(g_h.hdr.seq_num, h->hdr.ack, MAX_SHORT));
		h->hdr.ack = g_h.hdr.seq_num;
	}

	// First shift the ack bitfield over by the difference between the old ack and new ack
	h->hdr.ack_bits = h->hdr.ack_bits | (0x1 << (PROTO_diff(h->hdr.ack, g_h.hdr.seq_num, MAX_SHORT)));
}

// See which of your seq_num's your buddy has seen and remove them from the unacked_reliable_packets list
void PROTO_accept_acks(hostinfo_t *h)
{
	unsigned short ack,d;
	unsigned long ack_bits;
	int i;

	if(PROTO_more_recent(h->hdr.ack, g_h.hdr.seq_num, MAX_SHORT)) {
		printf("I have seen newer packets than this...\n");
		return;
	}

	// Copy them to a local variable once so they don't have to be dereferenced each time in the loops below
	ack = g_h.hdr.ack;
	ack_bits = g_h.hdr.ack_bits;

	// Loop through all the bits
	for(i = 0; i < NUMBITS; i++) {
		if(h->unacked_reliable_packet_map[i] != NULL) {
			d = h->unacked_reliable_packet_map[i]->seq_num;
			if(PROTO_more_recent(ack, d, MAX_SHORT) || d == ack) {
				if((ack_bits >> (PROTO_diff(ack,d,MAX_SHORT))) & 0x1) {
					h->unacked_reliable_packet_map[i] = NULL;
				}
			}
		}
	}
}

// Check if any packets sent to host 'h' appear to have been dropped and add
// them to the packet_queue to be resent
void PROTO_handle_timeout(hostinfo_t *h)
{
	packet_t *p;
	int i;

	// Go through all the packets that have not been ack'ed by this host
	for(i = 0; i < NUMBITS; i++) {
		p = h->unacked_reliable_packet_map[i];
		// If a timeout occured the packet is assumed to have been dropped and needs to be resent
		if(p && TIMER_is_timeout(&p->timer)) {
			printf("TIMEOUT\n\t");
			PROTO_queue_packet(h,p);
			h->unacked_reliable_packet_map[i] = NULL;
		}
	}
}

// Add a packet to the packet_queue which will be sent
void PROTO_queue_packet(hostinfo_t *h, packet_t *p)
{
	// Set packets new seq_num
	p->seq_num = h->hdr.seq_num;
	list_add(h->packet_queue, (void *)p);
	printf("h->packet_queue->len = %d\n",h->packet_queue->len);
}

// Send all the reliable packets that have been placed back into the packet_queue
void PROTO_send_packets(hostinfo_t *h)
{
	//TODO: this is one place congestion control can be taken into account
	packet_t *p;

	// Send any packets in the queue and remove them from it
	while (h->packet_queue->head != NULL) {
		p = (packet_t *)h->packet_queue->head->item;
		PROTO_send_reliable(h,p->b);

		list_del_head(h->packet_queue);
	}
}

// Handle resending of client packets
void PROTO_client_send_packets()
{
	if(clientinfo.state == STATE_CON) {
		PROTO_handle_timeout(clientinfo.info);
		PROTO_send_packets(clientinfo.info);
		PROTO_client_keepalive();
	}
}

// Handle resending of servere packets
void PROTO_server_send_packets()
{
	link_t *tmp;

	for(tmp = clientinfo_list->head; tmp != NULL; tmp = tmp->next) {
		PROTO_handle_timeout(((clientinfo_t *)tmp->item)->info);
		PROTO_send_packets(((clientinfo_t *)tmp->item)->info);
	}
}

void PROTO_client_keepalive()
{
	fixedbuf_t *b;

	if(TIMER_is_timeout(&clientinfo.info->keepalive)) {
		printf("keepalive\n");
		// Write packet
		b = buf_new_init(512);
		buf_memget(b, 11);
		buf_write_char(b, PTYPE_KEEPALIVE);

		PROTO_send_reliable(clientinfo.info, b);
	}
}

int PROTO_more_recent(unsigned short s1, unsigned short s2, unsigned short max_sequence)
{
	return (( s1 > s2 ) && ( s1 - s2 <= max_sequence/2 )) || (( s2 > s1 ) && ( s2 - s1 > max_sequence/2  ));
}

unsigned short PROTO_diff(unsigned short s1, unsigned short s2, unsigned short max_sequence)
{
	if(PROTO_more_recent(s1, s2, max_sequence))
		if(s2 > s1)
			return(max_sequence - s2 + s1 + 1);
		return(s1 - s2);
}

void PROTO_print_header_t(header_t *h)
{
	printf("header[%hu,%hu,%u,%hu,%lu]\n",h->size,
	                                  h->seq_num,
	                                  h->hid,
	                                  h->ack,
	                                  h->ack_bits
	                                  );
}

void PROTO_print_header(unsigned char *buf)
{
	printf("header[%hu,%hu,%u,%hu,%lu]\n",buf[0]<<8 | buf[1],
	                                  buf[2]<<8 | buf[3],
	                                  buf[4],
	                                  buf[5]<<8 | buf[6],
	                                  (unsigned long)(buf[7]<<24 | buf[8]<<16 | buf[9]<<8 | buf[10])
	                                  );
}
