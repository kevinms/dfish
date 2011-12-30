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

#include "ConnectionManager.h"

using namespace net;

ConnectionManager::ConnectionManager()
{
	
}

ConnectionManager::~ConnectionManager()
{
	
}

bool ConnectionManager::Start(Address &a, int options)
{
	if(!socket.Open(a, options))
	{
		fprintf(stderr,"Error: Could not open socket\n");
		return 0;
	}

	MaxConnections = 0;

	return 1;
}

void ConnectionManager::Stop()
{
	socket.Close();
}

void ConnectionManager::Connect(Address &a)
{
	Connection *c = new Connection(a);
	c->Connect();
	connections.push_back(c);
}

void ConnectionManager::Disconnect()
{
	int sz = connections.size();

	for(int i = 0; i < size; i++)
	{
		connections[i].Disconnect();
	}
}

bool ConnectionManager::Send(Packet &packet)
{
	int sz = connections.size();

	for(int i = 0; i < size; i++)
	{
		connections[i].SendPacket(packet);
	}
}

bool ConnectionManager::Receive()
{
	Address source;
	Packet packet(512);

	if(!socket.Receive(src, packet))
	{
		fprintf(stderr,"Error: could not receive packet\n");
		return false;
	}

	if(!ProcessPacket(source, packet))
	{
		fprintf(stderr,"Error: malformed packet\n");
		return false;
	}

	return true;
}

bool ConnectionManager::ProcessPacket(Address &source, Packet &packet)
{
	unsigned char protocolId;
	unsigned char uid;

	packet.Rewind();
	protocolId = packet.ReadChar();
	uid = packet.ReadChar();

	if(protocolId != NET_PROTOCOL_ID)
	{
		fprintf(stderr,"Error: wrong protocol id\n");
		return false;
	}

	Connection *conn = GetConnection(source, uid)

	if(!conn)
	{
		// Create a new connection and see if it is a connection request
		Connection *c = new Connection(source);
		c->ProcessPacket(packet);
	}

	
}

Connection *ConnectionManager::GetConnection(Address &a, unsigned char uid)
{
	int sz = connections.size();

	for(int i = 0; i < size; i++)
	{
		if(connections[i].Compare(a, uid))
		{
			return connections[i];
		}
	}

	return NULL;
}

void ConnectionManager::SetMaxConnections(int MaxConnections)
{
	this->MaxConnections = MaxConnections;
}

ConnectionManager
  + Start - create socket
  + Stop - destory socket
  + Connect - send a connection request, create new Connection
  + Disconnect - send a disconnect message, clear Connection
  + Send - given some packet data it determines which connections to send it to and calls its SendPacket method
  + Receive - receive a packet and pass it to the right Connection or process it if an unknown connection
  + ProcessPacket - process packets from unknown hosts, if maxconnections then send error message, else send packet to new connection and let it process it
  + SetMaxConnections - set how many connection requests the connectionmanager will accept

  . set of Connection objects for each connection
  . Socket
