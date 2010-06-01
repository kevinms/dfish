#include <stdio.h>

#include "net.h"
#include "buf.h"

int main()
{
	net_t *n;
	fixedbuf_t b;
	buf_init(&b, 512);
	int rv;

	n = NET_socket_server(NULL,"8888");
	if(!n)
		fprintf(stderr,"Error: could not create server socket\n");

	rv = NET_recv(n,&b);
	printf("Number of bytes recved: %d\n",rv);

	printf("%s\n",b.buf);

	return 0;
}
