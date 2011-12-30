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

#include "Connection.h"

#include <cstdio>

#include "../globals.h"

using namespace net;

Connection::Connection(Mode mode)
{
	this->mode = mode;
	state = Disconnected;
	id = 0;
}

Connection::~Connection()
{
	Close();
}

bool Connection::Open(Address &a)
{
	if(mode == Server)
	{
		if(!socket.Open(a, Socket::NonBlocking | Socket::Bind))
		{
			fprintf(stderr,"Error: Connection open failed\n");
			return false;
		}
	}
	else if(mode == Client)
	{
		if(!socket.Open(a, Socket::NonBlocking))
		{
			fprintf(stderr,"Error: Connection open failed\n");
			return false;
		}
	}
	return true;
}

void Connection::Close()
{
	socket.Close();
	state = Disconnected;
	id = 0;
}

bool Connection::Connect(Packet &packet)
{
	state = Connecting;
	packet.Rewind();
	packet.WriteChar(NET_PROTOCOL_ID);
	packet.WriteChar(id);

	socket.Send(address,packet);
	return true;
}

bool Connection::Discover(Packet &packet)
{
	state = Discovery;
	packet.Rewind();
	packet.WriteChar(NET_PROTOCOL_ID);
	packet.WriteChar(id);

	socket.Send(address,packet);
	return true;
}

void Connection::Disconnect(Packet &packet)
{
	state = Discovery;
	packet.Rewind();
	packet.WriteChar(NET_PROTOCOL_ID);
	packet.WriteChar(id);

	socket.Send(address,packet);
}

bool Connection::Listen()
{
	return true;
}
