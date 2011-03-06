// Copyright (C) 2011  Kevin Smith
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

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
