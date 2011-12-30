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

#include "Address.h"

#ifdef _WIN32
#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <string.h>

using namespace net;

Address::Address()
{
	ip = strdup("127.0.0.1");
	port = 8888;
}

Address::Address(const char *ip, unsigned short port)
{
	this->ip = strdup(ip);
	this->port = port;
}

Address::Address(void *addr, socklen_t addrlen)
{
	memcpy(&this->addr, addr, addrlen);
	this->addrlen = addrlen;

	ip = AddrToIP();
	port = AddrToPort();
}

char *Address::GetIP()
{
	return ip;
}

unsigned short Address::GetPort()
{
	return port;
}

void Address::SetAddr(void *addr, socklen_t addrlen)
{
	memcpy(&this->addr, addr, addrlen);
	this->addrlen = addrlen;

	ip = AddrToIP();
	port = AddrToPort();
}

const struct sockaddr *Address::GetAddr()
{
	return (struct sockaddr *)&addr;
}

socklen_t Address::GetAddrLen()
{
	return addrlen;
}

bool Address::operator==(const Address &other)
{
	return CompareIP(other) && ComparePort(other);
}

bool Address::operator!=(const Address &other)
{
	return !(*this == other);
}

// Compare the ips
bool Address::CompareIP(const Address &other)
{
	// IPv4 use sockaddr_in
	if(((struct sockaddr *)&addr)->sa_family == AF_INET && ((struct sockaddr *)&other.addr)->sa_family == AF_INET)
	{
		if(((struct sockaddr_in *)&addr)->sin_addr.s_addr == ((struct sockaddr_in *)&other.addr)->sin_addr.s_addr)
		{
			return true;
		}
	}
	// IPv6 use sockaddr_in6
	else if(((struct sockaddr *)&addr)->sa_family == AF_INET6 && ((struct sockaddr *)&other.addr)->sa_family == AF_INET6)
	{
		if(!strncmp( (char *)((struct sockaddr_in6 *)&addr)->sin6_addr.s6_addr, (char *)((struct sockaddr_in6 *)&other.addr)->sin6_addr.s6_addr, 16))
		{
			return true;
		}
	}
	return false;
}

// Compare the ports
bool Address::ComparePort(const Address &other)
{
	unsigned short p1;
	unsigned short p2;

	// IPv4 use sockaddr_in
	if(((struct sockaddr *)&addr)->sa_family == AF_INET && ((struct sockaddr *)&other.addr)->sa_family == AF_INET) {
		p1 = htons(((struct sockaddr_in *)&addr)->sin_port);
		p2 = htons(((struct sockaddr_in *)&other.addr)->sin_port);
		if(p1 == p2)
			return true;
	}
	// IPv6 use sockaddr_in6
	else if(((struct sockaddr *)&addr)->sa_family == AF_INET6 && ((struct sockaddr *)&other.addr)->sa_family == AF_INET6) {
		p1 = htons(((struct sockaddr_in6 *)&addr)->sin6_port);
		p2 = htons(((struct sockaddr_in6 *)&other.addr)->sin6_port);
		if(p1 == p2)
			return true;
	}

	return false;
}

// Print the IP address and Port number to stdout
void Address::print()
{
	printf("IP: %s:%d\n", ip, port);
}

// Gets a human readable string representation of the IP
char *Address::AddrToIP()
{
	char ip[INET6_ADDRSTRLEN];

#ifdef _WIN32
	if(!getnameinfo((struct sockaddr *)&addr, addrlen, ip, INET6_ADDRSTRLEN, NULL, 0, 0))
	{
		return strdup(ip);
	}
	printf("ERROR: cannot get ip\n");
	return NULL;
#else
	// IPv4 use sockaddr_in
	if(((struct sockaddr *)&addr)->sa_family == AF_INET)
	{
		if(inet_ntop(AF_INET, &(((struct sockaddr_in *)&addr)->sin_addr), ip, INET6_ADDRSTRLEN) != NULL)
		{
			return strdup(ip);
		}
	}
	// IPv6 use sockaddr_in6
	else if(((struct sockaddr *)&addr)->sa_family == AF_INET6)
	{
		if(inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)&addr)->sin6_addr.s6_addr), ip, INET6_ADDRSTRLEN) != NULL)
		{
			return strdup(ip);
		}
	}
	// Address family unkown
	printf("Address family unkown!\n");
	return NULL;
#endif
}

// Gets a human readable unsigned short of the PORT
unsigned short Address::AddrToPort()
{
	unsigned short port;

	// IPv4 use sockaddr_in
	if(((struct sockaddr *)&addr)->sa_family == AF_INET)
	{
		port = htons(((struct sockaddr_in *)&addr)->sin_port);
		return port;
		printf(":%d\n", port);
	}
	// IPv6 use sockaddr_in6
	else if(((struct sockaddr *)&addr)->sa_family == AF_INET6)
	{
		port = htons(((struct sockaddr_in6 *)&addr)->sin6_port);
		return port;
		printf(":%d\n", port);
	}
	// Address family unkown
	else
	{
		printf("Address family unkown!\n");
		return 0;
	}
}
