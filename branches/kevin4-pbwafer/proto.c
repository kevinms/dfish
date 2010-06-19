#include "proto.h"

#include "string.h"
#include "stdio.h"
#include "utils.h"
#include "version.h"
#include "console.h"

list_t *servinfo_list; // used by client
list_t *clientinfo_list; // used by server

servinfo_t servinfo;  // used by server
clientinfo_t clientinfo;  // used by client
hostinfo_t *c_host;  // used by client

fixedbuf_t g_buf;
hostinfo_t g_h;

hostinfo_t *PROTO_host_init()
{
	hostinfo_t *h;

	h = (hostinfo_t *)malloc(sizeof(*h));
	h->unacked_reliable_packets = list_init();
	h->queue_of_packets_to_send = list_init();

	return h;
}

hostinfo_t *PROTO_host_copy(hostinfo_t *src)
{
	hostinfo_t *h;

	h = PROTO_host_init();

	h->n = (net_t *)malloc(sizeof(*h->n));
	NET_copy(h->n, g_h.n);

	return h;
}

void PROTO_init()
{
	servinfo_list = list_init();
	clientinfo_list = list_init();

	buf_init(&g_buf,512);
	g_h.n = (net_t *)malloc(sizeof(*g_h.n));
	g_h.n->sockfd = 0;
	g_h.n->addrlen = sizeof(g_h.n->addr);
}

hostinfo_t *PROTO_socket_client(const char *node, const char *service)
{
	hostinfo_t *h;

	h = PROTO_host_init();
	h->n = NET_socket_client(node, service);
	g_h.n->sockfd = h->n->sockfd;

	return h;
}

hostinfo_t *PROTO_socket_server(const char *node, const char *service)
{
	hostinfo_t *h;

	h = PROTO_host_init();
	h->n = NET_socket_server(node, service);
	g_h.n->sockfd = h->n->sockfd;

	return h;
}

void PROTO_req_servinfo_ip(const char *node, const char *service)
{
	hostinfo_t *h;
	fixedbuf_t b;

	h = PROTO_socket_client(node, service);
	//TODO: if(!h)

	printf("req_servinfo\n");

	// Setup header
	buf_init(&b, 512);
	buf_memget(&b, 11);
	buf_write_char(&b, PTYPE_INFO);

	PROTO_send_reliable(h, &b);
}

void PROTO_set_servinfo(const char *name, unsigned short max_clients)
{
	servinfo.name = p_strcpy(name);
	servinfo.max_clients = max_clients;
}

void PROTO_set_clientinfo(int state, char *name)
{
	clientinfo.state = state;
	clientinfo.name = p_strcpy(name);
}

hostinfo_t *PROTO_connect(const char *node, const char *service)
{
	hostinfo_t *h;
	fixedbuf_t b;

	h = PROTO_socket_client(node, service);
	//TODO: if(!n)

	// Setup header
	buf_init(&b, 512);
	buf_memget(&b, 11);
	buf_write_char(&b, PTYPE_CONNECT);

	buf_write_char(&b, MAJOR_VERSION);
	buf_write_char(&b, MINOR_VERSION);
	buf_write_string(&b, clientinfo.name);

	clientinfo.info = h;

	PROTO_send_reliable(clientinfo.info, &b);

	return h;
}

// This function should only see packets from known hosts or connection requests
void PROTO_server_parse_DGRAM()
{
	int type;
	link_t *tmp;
	clientinfo_t *c;
	fixedbuf_t b;
	char *s;

	buf_init(&b, 512);

	type = buf_read_char(&g_buf);
	printf("type: %d\n",type);

	// Packet is an info request
	if(type == PTYPE_INFO) {
		printf("PTYPE_INFO\n");

		// Setup header
		buf_memget(&b, 11);
		buf_write_char(&b, PTYPE_INFO);

		// Write packet payload
		buf_write_string(&b, servinfo.name);
		buf_write_char(&b, MAJOR_VERSION);
		buf_write_char(&b, MINOR_VERSION);
		buf_write_short(&b, servinfo.max_clients);

		// Loop through and write all client names currently connected
		buf_write_short(&b, clientinfo_list->len);
		printf("len: %d\n",clientinfo_list->len);
		for(tmp = clientinfo_list->head; tmp != NULL; tmp = tmp->next) {
			buf_write_string(&b, ((clientinfo_t *)tmp->item)->name);
		}

		//TODO: send packet and set reliability type to (reliable) #maybe
		//TODO: check rv to make sure there were no errors
		PROTO_send(&g_h, &b);
	}

	// Packet is a connection Request
	if(type == PTYPE_CONNECT) {
		printf("PTYPE_CONNECT\n");
		//TODO: Check if there are any slots available on the server
		if(clientinfo_list->len == servinfo.max_clients) {
			buf_memget(&b, 11);
			buf_write_char(&b, PTYPE_ERROR);
			buf_write_char(&b, ERR_CONN_FULL);

			PROTO_send(&g_h, &b);
		}
		// Check if the client is using the same game version
		else if(buf_read_char(&g_buf) == MAJOR_VERSION && buf_read_char(&g_buf) == MINOR_VERSION) {
			printf("same version!\n");
			c = (clientinfo_t *)malloc(sizeof(*c));
			c->name = buf_read_string(&g_buf);

			//c->info = (hostinfo_t *)malloc(sizeof(*c->info));
			c->info = PROTO_host_init();
			c->info->n = (net_t *)malloc(sizeof(*c->info->n));
			NET_copy(c->info->n, g_h.n);
			c->info->hdr.hid = 10; //TODO: Generate a host id and move this to the payload

			list_add(clientinfo_list, (void *)c);

			// Build and send a reply packet
			buf_memget(&b, 11);
			buf_write_char(&b, PTYPE_CONNECT);
			buf_write_char(&b, 10); //TODO: Generate a host id

			//TODO: send packet and set reliability type to (reliable)
			//TODO: check rv to make sure there were no errors
			PROTO_send(&g_h, &b);
		} else {
			//TODO: send an error packet instead
			buf_memget(&b, 11);
			buf_write_char(&b, PTYPE_ERROR);
			buf_write_char(&b, ERR_CONN_VER);
			buf_write_char(&b, MAJOR_VERSION);
			buf_write_char(&b, MINOR_VERSION);

			PROTO_send(&g_h, &b);
		}
		return;
	}

	printf("Before\n");

	// Check if the packet is from a known host
	for(tmp = clientinfo_list->head; tmp != NULL; tmp = tmp->next) {
		if(PROTO_is_known_host(((clientinfo_t *)tmp->item)->info))
			break;
	}

	if(tmp == NULL)
		return;

	printf("After\n");

	c = (clientinfo_t *)tmp->item;
	//PROTO_accept_acks(c->info);

	if(type == PTYPE_KEEPALIVE) {
		//TODO: This was a reply from a server KEEPALIVE packet.  The reply
		//      tells the server that the client is still connected and we
		//      should reset the timeout for the client.
	}
	else if(type == PTYPE_MSG) {
		s = buf_read_string(&g_buf);
		printf("PTYPE_MSG: '%s'\n",s);
		PROTO_server_send_chat(c,s);
		//free(s);
	}
}

void PROTO_client_parse_DGRAM()
{
	int i;
	char type;
	servinfo_t *info;
	char *s, *s2;

	printf("PROTO_client_parse\n");

	type = buf_read_char(&g_buf);
	printf("type: %d\n",type);

	//TODO: Do an additional check to make sure we are even wanting to know
	//      info about servers right now.
	if(type == PTYPE_INFO)
	{
		info = (servinfo_t *)malloc(sizeof(*info));
		info->name =        buf_read_string(&g_buf);
		info->major_version = buf_read_char(&g_buf);
		info->minor_version = buf_read_char(&g_buf);

		info->max_clients = buf_read_short(&g_buf);

		// Read all the client names connected to the server
		info->num_clients = buf_read_short(&g_buf);
		info->clients = (const char **)malloc(sizeof(info->clients)*info->num_clients);
		for(i = 0; i < info->num_clients; i++)
			info->clients[i] = buf_read_string(&g_buf);

		//TODO: Calculate the ping
		//info->ping = ;

		list_add(servinfo_list, info);

		//TODO: remove this eventually, just a test print
		CONSOLE_print_no_update("----------Server Info---------");
		CONSOLE_print_no_update("name: %s", info->name);
		CONSOLE_print_no_update("version: %d", info->major_version);
		CONSOLE_print_no_update("version: %d", info->minor_version);
		CONSOLE_print_no_update("max_clients: %d", info->max_clients);
		CONSOLE_print_no_update("num_clients: %d", info->num_clients);
		CONSOLE_print_no_update("clients: ");
		for(i = 0; i < info->num_clients; i++)
			CONSOLE_print_no_update("    %s, ", info->clients[i]);
		//printf("\n");
		//printf("ping: %d\n", info->ping);
		CONSOLE_print_no_update("------------------------------");
		CONSOLE_update();
		return;
	}

	if(type == PTYPE_ERROR) {
		printf("ERROR: ");
		type = buf_read_char(&g_buf);

		if(type == ERR_CONN_VER)
			printf("Can not connect: wrong game version\n");
		else if(type == ERR_CONN_FULL)
			printf("Can not connect: server full\n");
		else
			printf("Unkown error: %d, send in a bugreport at code.google.com/p/dfish :)\n", type);
		return;
	}

	// Check if the packet is from a known host
	if(!PROTO_is_known_host(clientinfo.info))
		return;

	//PROTO_accept_acks(c_host);

	if(type == PTYPE_CONNECT) {
		printf("PTYPE_CONNECT\n");

		clientinfo.state = STATE_CON;
		//clientinfo.info->hdr.hid = g_h.hdr.hid;
		clientinfo.info->hdr.hid = buf_read_char(&g_buf);
		printf("hid: %d\n",clientinfo.info->hdr.hid);

		return;
	}

	if(type == PTYPE_KEEPALIVE) {
		
	}
	//TODO: Check if we are connected to this server
	if(type == PTYPE_MSG) {
		s = buf_read_string(&g_buf);
		s2 = buf_read_string(&g_buf);
		CONSOLE_print("%s: %s",s,s2);
	}
}

int PROTO_send_reliable(hostinfo_t *h, fixedbuf_t *b)
{
	// Store the packet and sequence number
	packet_t *p;
	p = (packet_t *)malloc(sizeof(*p));
	p->seq_num = h->hdr.seq_num;
	p->b = b;

	list_add(h->unacked_reliable_packets, (void *)p);

	//TODO: set some sort of timeout to resend the packet when reached
	return PROTO_send(h, b);
}

int PROTO_send_unreliable(hostinfo_t *h, fixedbuf_t *b)
{
	return PROTO_send(h, b);
	//TODO: free b!!!
}

int PROTO_send(hostinfo_t *h, fixedbuf_t *b)
{
	int cursize;
	int rv;

	// Fill in the NET header
	cursize = b->cursize;
	buf_reset(b);
	buf_write_short(b, cursize);
	buf_write_short(b, h->hdr.seq_num);
	buf_write_char(b, h->hdr.hid);
	buf_write_short(b, h->hdr.ack);     // ack (offset?)
	buf_write_long(b, h->hdr.ack_bits); // ack bitfield
	b->cursize = cursize;

	rv = NET_send(h->n, b);

	h->hdr.seq_num++;

	return rv;
}

int PROTO_recv()
{
	int rv;

	if(g_h.n->sockfd == 0)
		return -1;

	rv = NET_recv(g_h.n, &g_buf);

	g_h.hdr.size     = buf_read_short(&g_buf);
	g_h.hdr.seq_num  = buf_read_short(&g_buf);
	g_h.hdr.hid      = buf_read_char(&g_buf);
	g_h.hdr.ack      = buf_read_short(&g_buf);
	g_h.hdr.ack_bits = buf_read_long(&g_buf);

	//PROTO_is_known_host();
	// set a var saying if it is a known host or not
	return rv;
}

void PROTO_client_send_chat(const char *s)
{
	//CONSOLE_print_no_update(s);

	fixedbuf_t b;

	// Setup header
	buf_init(&b, 512);
	buf_memget(&b, 11);
	buf_write_char(&b, PTYPE_MSG);
	buf_write_string(&b, s);

	PROTO_send_reliable(clientinfo.info, &b);
}

void PROTO_server_send_chat(clientinfo_t *c, const char *s)
{
	fixedbuf_t b;
	link_t *tmp;

	// Setup header
	buf_init(&b, 512);
	buf_memget(&b, 11);
	buf_write_char(&b, PTYPE_MSG);
	buf_write_string(&b, c->name);
	buf_write_string(&b, s);

	// Check if the packet is from a known host
	for(tmp = clientinfo_list->head; tmp != NULL; tmp = tmp->next) {
		PROTO_send_reliable(((clientinfo_t *)tmp->item)->info, &b);
	}
}

char PROTO_is_known_host(hostinfo_t *h)
{
	printf("---------IsKnownHost----------\n");

	// Check IPs
	if(NET_ipcmp(h->n, g_h.n))
		return 0;
	printf("IP Match\n");

	// Check Ports
	if(NET_portcmp(h->n, g_h.n))
		return 0;
	printf("PORT Match\n");

	//TODO: put this as the first conditional statement because it has the least
	//      runtime of the 3 and should always be able to tell apart hosts
	//      unless they are forging thier HID or there is an error
	// Check HID
	printf("%d == %d\n",h->hdr.hid,g_h.hdr.hid);
	if(h->hdr.hid != g_h.hdr.hid)
		return 0;
	printf("HID Match\n");

	printf("------------------------------\n");

	return 1;
}

void PROTO_accept_acks(hostinfo_t *h)
{
	link_t *tmp;
	unsigned char ack;
	unsigned long ack_bits;
	int i;
	int size;

	ack = g_h.hdr.ack;
	ack_bits = g_h.hdr.ack_bits;

	// Go through unacked packet list and git rid of acked packets (need a hashmap)
	size = sizeof(ack_bits);
	for(i = 0; i < size; i++) {
		//printf("%c",((ack_bits >> i) & 0x1));
		if(((ack_bits >> i) & 0x1)) {
			for(tmp = h->unacked_reliable_packets->head; tmp != NULL; tmp = tmp->next) {
				if( ((packet_t *)tmp->item)->seq_num == ack-i ) {
					list_del_item(h->unacked_reliable_packets, tmp->item);
					break;
				}
			}
		}
	}
	printf("\n");
}
