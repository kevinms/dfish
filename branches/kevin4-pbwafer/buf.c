#include "buf.h"
#include <stdlib.h>
#include <string.h>

//TODO: remove this include, on here for debugging
#include <stdio.h>

void buf_init(fixedbuf_t *b, int size)
{
	b->maxsize = size;
	b->cursize = 0;
	b->curspot = 0;

	b->buf = (unsigned char *)malloc(size);
}

unsigned char *buf_memget(fixedbuf_t *b, int size)
{
	if(b->cursize + size > b->maxsize)
		return NULL;

	b->cursize += size;

	return (b->buf + (b->cursize - size));
}

int buf_write(fixedbuf_t *b, const char *c, int size)
{
	int i;

	for(i = 0; i < size && b->cursize < b->maxsize; i++, b->cursize++)
		b->buf[b->cursize] = c[i];

	return i;
}

void buf_write_char(fixedbuf_t *b, char c)
{
	unsigned char *buf;

	if( (buf = buf_memget(b,1)) )
		buf[0] = c;
}

void buf_write_string(fixedbuf_t *b, const char *s)
{
	int size = strlen(s)+1;
	int count;

	count = buf_write(b,s,size);

	if(count > 0 && count != size)
		b->buf[b->cursize-1] = '\0';
}

void buf_write_long(fixedbuf_t *b, int d)
{
	unsigned char *buf;

	if( (buf = buf_memget(b,4)) ) {
		buf[0] = (d>>24)&0xff;
		buf[1] = (d>>16)&0xff;
		buf[2] = (d>>8)&0xff;
		buf[3] = d&0xff;
	}
}

void buf_write_short(fixedbuf_t *b, int d)
{
	unsigned char *buf;

	if( (buf = buf_memget(b,2)) ) {
		buf[0] = (d>>8)&0xff;
		buf[1] = d&0xff;
	}
}

void buf_write_float(fixedbuf_t *b, float f)
{
	unsigned char *buf;
	int l;
	void *tmp;

	if( (buf = buf_memget(b,4)) ) {

		tmp = (void *)&f;
		l = *(int *)tmp;

		buf[0] = (l>>24)&0xff;
		buf[1] = (l>>16)&0xff;
		buf[2] = (l>>8)&0xff;
		buf[3] = l&0xff;
	}
}

void buf_write_float2(fixedbuf_t *b, float *f)
{
	unsigned char *buf;
	int l;
	void *tmp;

	if( (buf = buf_memget(b,8)) ) {
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

void buf_write_float3(fixedbuf_t *b, float *f)
{
	unsigned char *buf;
	int l;
	void *tmp;

	if( (buf = buf_memget(b,12)) ) {
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

char * buf_read(fixedbuf_t *b, int size)
{
	char *s;
	int i;

	s = (char *)malloc(size);

	for(i = 0; i < size && b->curspot < b->cursize; i++, b->curspot++)
		s[i] = b->buf[b->curspot];

	return s;
}

char buf_read_char(fixedbuf_t *b)
{
	if(b->curspot + 1 <= b->cursize)
		return b->buf[b->curspot++];

	return -1;
}

char *buf_read_string(fixedbuf_t *b)
{
	int i;

	for(i = b->curspot; b->buf[i] != '\0' && i < b->cursize; i++);

	if(b->buf[i] == '\0');
		return buf_read(b,i-b->curspot+1);
	return NULL;
}


int buf_read_short(fixedbuf_t *b)
{
	unsigned char *buf = b->buf+b->curspot;

	if(b->curspot+2 > b->cursize)
		return 0;

	b->curspot += 2;

	return buf[0]<<8 | buf[1];
}

int buf_read_long(fixedbuf_t *b)
{
	unsigned char *buf = b->buf+b->curspot;

	if(b->curspot+4 > b->cursize)
		return 0;

	b->curspot += 4;

	return buf[0]<<24 | buf[1]<<16 | buf[2]<<8 | buf[3];
}

float buf_read_float(fixedbuf_t *b)
{
	unsigned char *buf = b->buf+b->curspot;
	float *f;

	if(b->curspot+4 > b->cursize)
		return 0.0;

	b->curspot += 4;

	int l = buf[0]<<24 | buf[1]<<16 | buf[2]<<8 | buf[3];
	f = (float *)&l;

	return *f;
}

void buf_read_float2(fixedbuf_t *b, float *f)
{
	unsigned char *buf = b->buf+b->curspot;
	union
	{
		int l[2];
		float f2[2];
	} data;

	if(b->curspot+8 > b->cursize)
		return;

	b->curspot += 8;

	data.l[0] = buf[0]<<24 | buf[1]<<16 | buf[2]<<8 | buf[3];
	data.l[1] = buf[4]<<24 | buf[5]<<16 | buf[6]<<8 | buf[7];

	f[0] = data.f2[0];
	f[1] = data.f2[1];
}

void buf_read_float3(fixedbuf_t *b, float *f)
{
	unsigned char *buf = b->buf+b->curspot;
	union
	{
		int l[3];
		float f2[3];
	} data;

	if(b->curspot+12 > b->cursize)
		return;

	b->curspot += 12;

	data.l[0] = buf[0]<<24 | buf[1]<<16 | buf[2]<<8  | buf[3];
	data.l[1] = buf[4]<<24 | buf[5]<<16 | buf[6]<<8  | buf[7];
	data.l[2] = buf[8]<<24 | buf[9]<<16 | buf[10]<<8 | buf[11];

	f[0] = data.f2[0];
	f[1] = data.f2[1];
	f[2] = data.f2[2];
}
