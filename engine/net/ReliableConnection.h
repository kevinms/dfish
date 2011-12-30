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

// Reliability Layer Packet Header
// +--------+--------+--------+---------+--------+----------+------+
// |  UDP   |  Conn  | Packet | seq_num |  ack   |   ack    | Data |
// | Header | Header |  Size  |         |        | bitfield |      |
// +--------+--------+--------+---------+--------+----------+------+
//     8         2        2        2        2         4        n
// 10 bytes ( +8 UDP + 2 Conn)

// Event Layer Packet Header
// +--------+--------+--------+------+------+
// |  UDP   |  Conn  |  Reli  | Type | Data |
// | Header | Header | Header |      |      |
// +--------+--------+--------+------+------+
//     8         2        2       1     n
// 1 byte ( +8 UDP + 2 Conn + 10 Reli)

#ifndef __RELIABLECONNECTION_H
#define __RELIABLECONNECTION_H

#include "Connection.h"

namespace net
{
	class ReliableConnection : Connection
	{
		public:
			ReliableConnection();
			~ReliableConnection();

		private:
			
	};
}

#endif // !__RELIABLECONNECTION_H

