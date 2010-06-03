#include "proto.h"

#include "string.h"
#include "stdio.h"
#include "utils.h"
#include "version.h"

list_t *servinfo_list; // used by client
list_t *clientinfo_list; // used by server

servinfo_t servinfo;  // used by server
clientinfo_t clientinfo;  // used by client

void PROTO_init()
{
	servinfo_list = list_init();
	clientinfo_list = list_init();
}

void PROTO_req_servinfo_ip(const char *node, const char *service)
{
	net_t *n;
	fixedbuf_t b;

	n = NET_socket_client(node, service);
	//TODO: if(!n)

	printf("req_servinfo\n");

	// Setup NET+PROTO header
	buf_init(&b, 512);
	buf_memget(&b, 5);
	buf_write_char(&b, PTYPE_INFO);
	buf_write_short(&b, 0);

	NET_send_reliable(n, &b);

	//TODO: what happens if two clients ask for servinfo at the same time?
	//      answer: this function only sends a single packet it is recved elsewhere
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

net_t *PROTO_connect(const char *node, const char *service)
{
	net_t *n;
	fixedbuf_t b;

	n = NET_socket_client(node, service);
	//TODO: if(!n)

	// Setup NET+PROTO header
	buf_init(&b, 512);
	buf_memget(&b, 5);
	buf_write_char(&b, PTYPE_CONNECT);
	buf_write_short(&b, 0);

	buf_write_char(&b, MAJOR_VERSION);
	buf_write_char(&b, MINOR_VERSION);
	buf_write_string(&b, clientinfo.name);

	NET_send_reliable(n, &b);

	return n;
}

// This function should only see packets from known hosts or connection requests
void PROTO_server_parse_DGRAM()
{
	int type;
	link_t *tmp;
	unsigned short cid;
	clientinfo_t *c;
	fixedbuf_t b;

	net_t n;
	memcpy((void *)&n.addr, (void *)&g_addr, g_addrlen);
	n.addrlen = g_addrlen;

	buf_init(&b, 512);

	buf_clear(&g_buf);

	printf("packetsize: %d\n",buf_read_short(&g_buf));
	printf("reliability: %d\n",buf_read_char(&g_buf));
	printf("sequence#: %d\n",buf_read_short(&g_buf));

	type = buf_read_char(&g_buf);
	cid  = buf_read_short(&g_buf);

	printf("tehehe %d\n",type);

	// Packet is an info request
	if(type == PTYPE_INFO) {
		printf("PTYPE_INFO\n");

		// Setup NET+PROTO header
		buf_memget(&b, 5);
		buf_write_char(&b, PTYPE_INFO);
		buf_write_short(&b, 0);

		// Write packet payload
		buf_write_string(&b, servinfo.name);
		printf("servinfo.name: %s\n",servinfo.name);
		buf_write_char(&b, MAJOR_VERSION);
		buf_write_char(&b, MINOR_VERSION);
		buf_write_short(&b, servinfo.max_clients);

		// Loop through and write all client names currently connected
		buf_write_short(&b, clientinfo_list->len);
		printf("len: %d\n",clientinfo_list->len);
		for(tmp = clientinfo_list->head; tmp != NULL; tmp = tmp->next) {
			buf_write_string(&b, ((clientinfo_t *)tmp->item)->name);
			printf("name: %s\n",((clientinfo_t *)tmp->item)->name);
		}

		//TODO: send packet and set reliability type to (reliable) #maybe
		//TODO: check rv to make sure there were no errors
		NET_send(&n, &b);
	}

	// Packet is a connection Request
	if(type == PTYPE_CONNECT) {
		printf("PTYPE_CONNECT\n");
		//TODO: Check if there are any slots available on the server
		if(clientinfo_list->len == servinfo.max_clients)
			return;

		// Check if the client is using the same game version
		if(buf_read_char(&g_buf) == MAJOR_VERSION && buf_read_char(&g_buf) == MINOR_VERSION){
			printf("same version!\n");
			c = (clientinfo_t *)malloc(sizeof(*c));
			c->name = buf_read_string(&g_buf);

			list_add(clientinfo_list, (void *)c);

			// Build and send a reply packet
			buf_memget(&b, 5);
			buf_write_char(&b, PTYPE_CONNECT);
			buf_write_short(&b, 10);  //TODO: Generate a client id

			//TODO: send packet and set reliability type to (reliable)
			//TODO: check rv to make sure there were no errors
			NET_send(&n, &b);
		} else {
			//TODO: send an error packet instead
		}
	}

	//TODO: Check if the packet is from a known host

	if(type == PTYPE_KEEPALIVE) {
		//TODO: This was a reply from a server KEEPALIVE packet.  The reply
		//      tells the server that the client is still connected and we
		//      should reset the timeout for the client.
	}
}

void PROTO_client_parse_DGRAM()
{
	int i;
	char type;
	unsigned short cid;
	servinfo_t *info;

	printf("PROTO_client_parse\n");

	//TODO: Where will all the data about the client be saved?
	buf_clear(&g_buf);
	printf("packetsize: %d\n",buf_read_short(&g_buf));
	printf("reliability: %d\n",buf_read_char(&g_buf));
	printf("sequence#: %d\n",buf_read_short(&g_buf));

	type = buf_read_char(&g_buf);
	printf("type %d\n",type);
	cid = buf_read_short(&g_buf);

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
		printf("Server Info\n");
		printf("name: %s\n", info->name);
		printf("version: %d\n", info->major_version);
		printf("version: %d\n", info->minor_version);
		printf("max_clients: %d\n", info->max_clients);
		printf("num_clients: %d\n", info->num_clients);
		printf("clients: ");
		for(i = 0; i < info->num_clients; i++)
			printf("%s, ", info->clients[i]);
		printf("\n");
		//printf("ping: %d\n", info->ping);
	}

	if(type == PTYPE_CONNECT) {
		printf("PTYPE_CONNECT\n");
		printf("cid: %d\n",cid);

		clientinfo.id = cid;
	}

	if(type == PTYPE_KEEPALIVE) {
		
	}
	//TODO: Check if we are connected to this server
}
