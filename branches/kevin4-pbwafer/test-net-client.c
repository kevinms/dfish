#include <stdio.h>

#include "net.h"

int main()
{
	net_t *n;
	fixedbuf_t b;
	int rv;

	buf_init(&b,512);
	buf_write_string(&b, "Hello World!");

	n = NET_socket_client("127.0.0.1","8888");

	rv = NET_send(n, &b);
	printf("Number of bytes sent: %d\n", rv);

	return 0;
}
