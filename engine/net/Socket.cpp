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

#include "Socket.h"

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

#ifdef _WIN32
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

using namespace net;

Socket::Socket()
{
	options = 0;
	#ifdef WIN32
	sockfd = INVALID_SOCKET;
	#else
	sockfd = -1;
	#endif
}

Socket::~Socket()
{
	Close();
}

bool Socket::Open(Address &a, int options)
{
	int rv;
	char port_str[16];
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct addrinfo *p;

	// Fill the hints struct with IP version agnostic constants and UDP socktype
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	// Says that it will be a server
	if(options & Bind)
	{
		hints.ai_flags = AI_PASSIVE;
	}

	// Get a listing of interfaces we may be able to make a socket on
	sprintf(port_str,"%d",a.GetPort());
	if((rv = getaddrinfo(a.GetIP(), port_str, &hints, &servinfo)) != 0)
	{
		fprintf(stdout, "getaddrinfo error: %s\n", gai_strerror(rv));
		return false;
	}

	// loop through all the results and make a socket
	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("talker: socket");
			continue;
		}

		if(options & Bind)
		{
			// Lets us bind to a socket even if it appears to be in use by another program
			int yes = 1;
			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
				perror("setsockopt");
				return false;
			}

			if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				#ifdef _WIN32
				closesocket(sockfd);
				#else
				close(sockfd);
				#endif
				perror("server: bind");
				continue;
			}
		}

		break;
	}

	if(p == NULL)
	{
		fprintf(stderr, "Error: client failed to bind socket\n");
		return false;
	}
	freeaddrinfo(servinfo);

	// Set non-blocking io
	if(options & NonBlocking)
	{
		#ifdef WIN32
		DWORD nonBlocking = 1;
		if(ioctlsocket(sockfd, FIONBIO, &nonBlocking ) != 0)
		{
			fprintf( "Error: failed to set non-blocking socket\n");
			Close();
			return false;
		}
		#else
		if(fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL)|O_NONBLOCK) == -1)
		{
			fprintf(stderr,"Error: failed to set non-blocking socket\n");
			Close();
			return false;
		}
		#endif
	}

	// Set SO_BROADCAST so it can use a broadcast address
	if(options & Broadcast)
	{
		int broadcast = 1;
		if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
			perror("Error: setsockopt (SO_BROADCAST)");
			Close();
			return false;
		}
	}

	a.SetAddr(p->ai_addr,p->ai_addrlen);

	return true;
}

void Socket::Close()
{
	if(IsOpen())
	{
		#ifdef WIN32
		closesocket(sockfd);
		sockfd = INVALID_SOCKET;
		#else
		close(sockfd);
		sockfd = -1;
		#endif
	}
}

bool Socket::IsOpen()
{
	#ifdef WIN32
	return sockfd != INVALID_SOCKET;
	#else
	return sockfd >= 0;
	#endif
}

int Socket::Send(Address &dest, Packet &packet)
{
	int sent_bytes;

	packet.Check();

	if(!IsOpen())
		return false;

	if ((sent_bytes = sendto(sockfd, packet.GetBuf(), packet.GetSize(), 0, dest.GetAddr(), dest.GetAddrLen())) == -1) {
		perror("Error: sendto failed");
	}

	if(sent_bytes != (int)packet.GetSize())
	{
		fprintf(stderr,"Error: partial send\n");
	}

	return sent_bytes;
}

bool Socket::Receive(Address &src, Packet &packet)
{
	int received_bytes;
	sockaddr_in from;
	socklen_t fromLength = sizeof(from);

	packet.Check();

	if ( !IsOpen() )
		return false;

	received_bytes = recvfrom(sockfd, packet.GetBuf(), packet.GetSize(), 0, (struct sockaddr *)&from, &fromLength);

	if ( received_bytes <= 0 )
		return false;

	src.SetAddr(&from, fromLength);
	packet.SetSize(received_bytes);

	return true;
}

bool Socket::SetBroadcast(int broadcast)
{
	if(broadcast > 0)
	{
		broadcast = 1;
	}
	else
	{
		broadcast = 0;
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast) == -1) {
		perror("Error: setsockopt (SO_BROADCAST)");
		Close();
		return false;
	}
	return true;
}
