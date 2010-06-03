#include <stdio.h>

#include "buf.h"
#include "net.h"
#include "proto.h"

int main()
{
/*
	int rv;

	rv = NET_recv(n,&b);
	printf("Number of bytes recved: %d\n",rv);

	printf("%s\n",b.buf);
*/

	NET_init();
	PROTO_init();

	net_t *n;

	PROTO_set_servinfo("pbwafer test server", 3);

	n = NET_socket_server(NULL,"8888");
	if(!n)
		fprintf(stderr,"Error: could not create server socket\n");

	for(;;) {
		if(g_sockfd != 0) // This tells us if we have any sockets ready to recv on
			if(NET_recv() > 0)
				PROTO_server_parse_DGRAM();
	}

	return 0;
}
