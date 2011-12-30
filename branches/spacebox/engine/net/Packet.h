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

#ifndef __PACKET_H
#define __PACKET_H

namespace net
{
	class Packet
	{
		public:
			Packet(int size);
			~Packet();
			void Rewind();
			void Empty();
			void Check();
			unsigned int GetSize();
			bool SetSize(unsigned int size);
			unsigned int GetMaxSize();
			unsigned int GetPosition();
			unsigned char *GetBuf();
			bool Seek(unsigned int position);

			unsigned char *MemGet(int size);

			// Write
			int  Write(const char *c, int size);
			void WriteChar(char c);
			void WriteString(const char *s);
			void WriteLong(int d);
			void WriteShort(int d);
			void WriteFloat(float f);
			void WriteFloat2(float *f);
			void WriteFloat3(float *f);

			// Read
			char *Read(int size);
			char  ReadChar();
			char *ReadString();
			int   ReadShort();
			int   ReadLong();
			float ReadFloat();
			void  ReadFloat2(float *f);
			void  ReadFloat3(float *f);

		private:
			unsigned int curspot;
			unsigned int cursize;
			unsigned int maxsize;
			unsigned char *buf;
	};
}

#endif // !__PACKET_H
