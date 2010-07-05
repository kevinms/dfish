#include <stdio.h>

#include "pbwafer/pbwafer.h"
#include "pbwafer/buf.h"
#include "pbwafer/net.h"
#include "pbwafer/proto.h"

int main()
{
	hostinfo_t *h;

	PBWAFER_init(INIT_NET|INIT_PROTO);

	PROTO_set_servinfo("pbwafer test server", 200);

	h = PROTO_socket_server(NULL,"8888");
	if(!h)
		fprintf(stderr,"Error: could not create server socket\n");

	for(;;) {
		if(PROTO_recv() > 0)
			PROTO_server_parse_DGRAM();

		PROTO_server_send_packets();
	}
	return 0;
}
