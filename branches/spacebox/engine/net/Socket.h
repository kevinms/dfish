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

#ifndef __SOCKET_H
#define __SOCKET_H

#include "Address.h"
#include "Packet.h"

namespace net
{
	class Socket
	{
		public:
			enum Options
			{
				NonBlocking = 1,
				Broadcast = 2,
				Bind = 4
			};

			Socket();
			~Socket();

			bool Open(Address &a, int options = NonBlocking);
			void Close();
			bool IsOpen();
			int Send(Address &dest, Packet &packet);
			bool Receive(Address &src, Packet &packet);
			bool SetBroadcast(int broadcast);

		private:
			int sockfd;
			int options;
	};
}

#endif // !__SOCKET_H
