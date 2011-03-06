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

#ifndef _BUF_H_
#define _BUF_H_

//TODO: Take into account endianess

//TODO: Always malloc 8 extra bytes for the packet header.  One of the NET or
//      PROTO functions can fill in the header
typedef struct fixedbuf_s
{
	unsigned int curspot;
	unsigned int cursize;
	unsigned int maxsize;
	unsigned char *buf;
} fixedbuf_t;

void buf_init(fixedbuf_t *b, int size);
fixedbuf_t *buf_new_init(int size);
void buf_clear(fixedbuf_t *b);
void buf_reset(fixedbuf_t *b);

unsigned char *buf_memget(fixedbuf_t *b, int size);

// Write to a fixedbuf
int  buf_write       (fixedbuf_t *b, const char *c, int size);
void buf_write_char  (fixedbuf_t *b, char c);
void buf_write_string(fixedbuf_t *b, const char *s);
void buf_write_long  (fixedbuf_t *b, int d);
void buf_write_short (fixedbuf_t *b, int d);
void buf_write_float (fixedbuf_t *b, float f);
void buf_write_float2(fixedbuf_t *b, float *f);
void buf_write_float3(fixedbuf_t *b, float *f);

// Read from a fixedbuf
char *buf_read       (fixedbuf_t *b, int size);
char  buf_read_char  (fixedbuf_t *b);
char *buf_read_string(fixedbuf_t *b);
int   buf_read_short (fixedbuf_t *b);
int   buf_read_long  (fixedbuf_t *b);
float buf_read_float (fixedbuf_t *b);
void  buf_read_float2(fixedbuf_t *b, float *f);
void  buf_read_float3(fixedbuf_t *b, float *f);

#endif /* !_BUF_H_ */
