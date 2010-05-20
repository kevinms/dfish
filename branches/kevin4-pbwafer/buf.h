#ifndef _BUF_H_
#define _BUF_H_

//TODO: Take into account endianess

typedef struct fixedbuf_s
{
	unsigned int curspot;
	unsigned int cursize;
	unsigned int maxsize;
	unsigned char *buf;
} fixedbuf_t;

void buf_init(fixedbuf_t *b, int size);

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
