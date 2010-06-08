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

	hostinfo_t *h;

	PROTO_set_servinfo("pbwafer test server", 3);

	h = PROTO_socket_server(NULL,"8888");
	if(!h)
		fprintf(stderr,"Error: could not create server socket\n");

	for(;;)
		if(PROTO_recv() > 0)
			PROTO_server_parse_DGRAM();

	return 0;
}
