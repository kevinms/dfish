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

#ifndef __ADDRESS_H
#define __ADDRESS_H

#ifdef _WIN32
#include <Winsock2.h>
#else
#include <netdb.h>
#endif

#ifdef _WIN32
typedef int socklen_t;
#endif

namespace net
{
	class Address
	{
		public:
			Address();
			Address(const char *ip, unsigned short port);
			Address(void *addr, socklen_t addrlen);

			char *GetIP();
			unsigned short GetPort();
			void SetAddr(void *addr, socklen_t addrlen);
			const struct sockaddr *GetAddr();
			socklen_t GetAddrLen();

			bool operator==(const Address &other);
			bool operator!=(const Address &other);
			bool CompareIP(const Address &other);
			bool ComparePort(const Address &other);

			void print();
			char *AddrToIP();
			unsigned short AddrToPort();

		private:
			char *ip;
			unsigned short port;

			struct sockaddr_storage addr;
			socklen_t addrlen;
	};
}

#endif // !__ADDRESS_H
