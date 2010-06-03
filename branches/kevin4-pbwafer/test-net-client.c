#include <stdio.h>

#include "input.h"
#include "buf.h"
#include "cmd.h"
#include "net.h"
#include "proto.h"
#include "vid.h"

net_t *netserv;

void req_servinfo()
{
	PROTO_req_servinfo_ip("127.0.0.1", "8888");
	printf("hi\n");
}

void req_connect()
{
	netserv = NET_socket_client("127.0.0.1","8888");
	PROTO_connect("127.0.0.1", "8888");
}

void send_msg()
{
	//PROTO_client_send_msg("bob");
}

int main()
{

	cmd_t g_cmds[3] = 
	{
		{{KM_NONE,K_i},req_servinfo,NULL},
		{{KM_NONE,K_c},req_connect,NULL},
		{{KM_NONE,K_i},send_msg,NULL}
	};

	NET_init();
	PROTO_init();

	// initialize the video system
	vidmode_t vm;
	vm.width = 700;
	vm.height = 480;
	vm.bpp = 32;
	VID_init();
	VID_init_mode(&vm);
	VID_set_caption("pbwafer - test driver");

	atexit(SDL_Quit);

	input_init();

/*
	net_t *n;
	fixedbuf_t b;
	int rv;

	buf_init(&b,512);
	buf_write_string(&b, "Hello World!");

	rv = NET_send(n, &b);
	printf("Number of bytes sent: %d\n", rv);
*/
	// Game Loop
	cmd_t *c = NULL;
	int ret;

	PROTO_set_clientinfo(STATE_NOP,"pughar");

	for(;;) {
		//printf("%d\n",g_sockfd);
		if(g_sockfd != 0) // This tells us if we have any sockets ready to recv on
			if(NET_recv() > 0) {
				PROTO_client_parse_DGRAM();
			//printf("bob");
		}

		ret = input_handle();
			if(ret == 2) {
				// Check the global commands
				c = CMD_find_key_array(g_cmds,&g_chain,3);
				if(c != NULL) {
					CMD_do(c);
				}
			} else if(ret == 0){
				return 0;
			}
	}
	return 0;
}
