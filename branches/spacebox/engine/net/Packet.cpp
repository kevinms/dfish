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

#include "Packet.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

//TODO: remove this include, on here for debugging
#include <stdio.h>

using namespace net;

Packet::Packet(int size)
{
	maxsize = size;
	cursize = 0;
	curspot = 0;

	buf = (unsigned char *)malloc(size);
}

Packet::~Packet()
{
	if(buf)
		free(buf);
}

void Packet::Rewind()
{
	curspot = 0;
}

void Packet::Empty()
{
	cursize = 0;
	curspot = 0;
}

void Packet::Check()
{
	assert(buf);
	assert(cursize > 0);
}

unsigned int Packet::GetSize()
{
	return cursize;
}

bool Packet::SetSize(unsigned int size)
{
	if(size < maxsize)
	{
		cursize = size;
		return true;
	}
	return false;
}

unsigned int Packet::GetMaxSize()
{
	return maxsize;
}

unsigned int Packet::GetPosition()
{
	return curspot;
}

unsigned char *Packet::GetBuf()
{
	return buf;
}

bool Packet::Seek(unsigned int position)
{
	if(position < maxsize)
	{
		curspot = position;
		return true;
	}
	return false;
}

unsigned char *Packet::MemGet(int size)
{
	if(cursize + size > maxsize)
		return NULL;

	cursize += size;

	return (buf + (cursize - size));
}

int Packet::Write(const char *c, int size)
{
	int i;

	for(i = 0; i < size && cursize < maxsize; i++, cursize++)
		buf[cursize] = c[i];

	return i;
}

void Packet::WriteChar(char c)
{
	unsigned char *buf;

	if( (buf = MemGet(1)) )
		buf[0] = c;
}

void Packet::WriteString(const char *s)
{
	int size = strlen(s)+1;
	int count;

	count = Write(s,size);

	if(count > 0 && count != size)
		buf[cursize-1] = '\0';
}

void Packet::WriteLong(int d)
{
	unsigned char *buf;

	if( (buf = MemGet(4)) ) {
		buf[0] = (d>>24)&0xff;
		buf[1] = (d>>16)&0xff;
		buf[2] = (d>>8)&0xff;
		buf[3] = d&0xff;
	}
}

void Packet::WriteShort(int d)
{
	unsigned char *buf;

	if( (buf = MemGet(2)) ) {
		buf[0] = (d>>8)&0xff;
		buf[1] = d&0xff;
	}
}

void Packet::WriteFloat(float f)
{
	unsigned char *buf;
	int l;
	void *tmp;

	if( (buf = MemGet(4)) ) {

		tmp = (void *)&f;
		l = *(int *)tmp;

		buf[0] = (l>>24)&0xff;
		buf[1] = (l>>16)&0xff;
		buf[2] = (l>>8)&0xff;
		buf[3] = l&0xff;
	}
}

void Packet::WriteFloat2(float *f)
{
	unsigned char *buf;
	int l;
	void *tmp;

	if( (buf = MemGet(8)) ) {
		tmp = (void *)&f[0];
		l = *(int *)tmp;

		buf[0] = (l>>24)&0xff;
		buf[1] = (l>>16)&0xff;
		buf[2] = (l>>8)&0xff;
		buf[3] = l&0xff;

		tmp = (void *)&f[1];
		l = *(int *)tmp;

		buf[4] = (l>>24)&0xff;
		buf[5] = (l>>16)&0xff;
		buf[6] = (l>>8)&0xff;
		buf[7] = l&0xff;
	}
}

void Packet::WriteFloat3(float *f)
{
	unsigned char *buf;
	int l;
	void *tmp;

	if( (buf = MemGet(12)) ) {
		tmp = (void *)&f[0];
		l = *(int *)tmp;

		buf[0] = (l>>24)&0xff;
		buf[1] = (l>>16)&0xff;
		buf[2] = (l>>8)&0xff;
		buf[3] = l&0xff;

		tmp = (void *)&f[1];
		l = *(int *)tmp;

		buf[4] = (l>>24)&0xff;
		buf[5] = (l>>16)&0xff;
		buf[6] = (l>>8)&0xff;
		buf[7] = l&0xff;

		tmp = (void *)&f[2];
		l = *(int *)tmp;

		buf[8] = (l>>24)&0xff;
		buf[9] = (l>>16)&0xff;
		buf[10] = (l>>8)&0xff;
		buf[11] = l&0xff;
	}
}

char * Packet::Read(int size)
{
	char *s;
	int i;

	s = (char *)malloc(size);

	for(i = 0; i < size && curspot < cursize; i++, curspot++)
		s[i] = buf[curspot];

	return s;
}

char Packet::ReadChar()
{
	if(curspot + 1 <= cursize)
		return buf[curspot++];

	return -1;
}

char *Packet::ReadString()
{
	unsigned int i;

	for(i = curspot; buf[i] != '\0' && i < cursize; i++);

	if(buf[i] == '\0');
		return Read(i-curspot+1);
	return NULL;
}

int Packet::ReadShort()
{
	unsigned char *b = buf+curspot;

	if(curspot+2 > cursize)
		return 0;

	curspot += 2;

	return b[0]<<8 | b[1];
}

int Packet::ReadLong()
{
	unsigned char *b = buf+curspot;

	if(curspot+4 > cursize)
		return 0;

	curspot += 4;

	return b[0]<<24 | b[1]<<16 | b[2]<<8 | b[3];
}

float Packet::ReadFloat()
{
	unsigned char *b = buf+curspot;
	union
	{
		int l;
		float f;
	} data;

	if(curspot+4 > cursize)
		return 0.0;

	curspot += 4;

	data.l = b[0]<<24 | b[1]<<16 | b[2]<<8 | b[3];

	return data.f;
}

void Packet::ReadFloat2(float *f)
{
	unsigned char *b = buf+curspot;
	union
	{
		int l[2];
		float f2[2];
	} data;

	if(curspot+8 > cursize)
		return;

	curspot += 8;

	data.l[0] = b[0]<<24 | b[1]<<16 | b[2]<<8 | b[3];
	data.l[1] = b[4]<<24 | b[5]<<16 | b[6]<<8 | b[7];

	f[0] = data.f2[0];
	f[1] = data.f2[1];
}

void Packet::ReadFloat3(float *f)
{
	unsigned char *b = buf+curspot;
	union
	{
		int l[3];
		float f2[3];
	} data;

	if(curspot+12 > cursize)
		return;

	curspot += 12;

	data.l[0] = b[0]<<24 | b[1]<<16 | b[2]<<8  | b[3];
	data.l[1] = b[4]<<24 | b[5]<<16 | b[6]<<8  | b[7];
	data.l[2] = b[8]<<24 | b[9]<<16 | b[10]<<8 | b[11];

	f[0] = data.f2[0];
	f[1] = data.f2[1];
	f[2] = data.f2[2];
}
