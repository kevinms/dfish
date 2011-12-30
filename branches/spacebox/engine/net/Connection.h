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

// Connection Layer Packet Header
// +--------+----------+------+------+
// |  UDP   | Protocol | Host | Data |
// | Header |    ID    |  ID  |      |
// +--------+----------+------+------+
//      8        1        1      n
// 2 bytes ( +8 UDP)

#ifndef __CONNECTION_H
#define __CONNECTION_H

#include "Address.h"
#include "Socket.h"
#include "Packet.h"

// Virtual Connection
//   1. Starts in disconnected state
//   2. When it is disconnected it can go into lan discovery state
//   3. It broadcasts a lan discovery packet to the world
//   4. Waits for a reply
//   5. After a certain timeout it should go back to disconnected state
//   6. It can can go into connecting state if connect() is called
//   7. Once the server replies with a hostid or error

namespace net
{
	class Connection
	{
		public:
			enum Mode
			{
				None,
				Client,
				Server
			};

			enum State
			{
				Disconnected,
				Discovery,
				Listening,
				Connecting,
				ConnectFail,
				Connected
			};

			Connection(Mode mode);
			~Connection();

			bool Open(Address &a);
			void Close();

			bool Connect(Packet &packet);
			bool Discover(Packet &packet);
			void Disconnect(Packet &packet);

			bool Listen();

			State GetState();

		private:
			double timeout;
			Address address;
			Socket socket;
			Mode mode;
			State state;
			unsigned char id;
	};
}

#endif // !__CONNECTION_H
