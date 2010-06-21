#include <stdio.h>
#include "buf.h"

int main()
{
	char str[6];
	fixedbuf_t b;
	fixedbuf_t b2;
	char *s;
	int d;
	float f[3];
	float f2[3];

	//TEST: buffer bounds checking
	printf("buffer bounds checking:\n");
	buf_init(&b2,7);
	buf_write_string(&b2,"salvation");
	s = buf_read_string(&b2);
	printf("write: %s\n","salvation");
	printf("read:  %s\n\n",s);

	buf_init(&b,256);

	//TEST: w/r char
	buf_write_char(&b,'m');
	buf_write_char(&b,'r');
	buf_write_char(&b,'p');
	buf_write_char(&b,'a');
	buf_write_char(&b,'l');
	str[0] = buf_read_char(&b);
	str[1] = buf_read_char(&b);
	str[2] = buf_read_char(&b);
	str[3] = buf_read_char(&b);
	str[4] = buf_read_char(&b);
	str[5] = '\0';
	printf("write: %s\n","mrpal");
	printf("read:  %s\n",str);

	//TEST: w/r string
	buf_write_string(&b,"How is dwarf fortress going?");
	s = buf_read_string(&b);
	printf("write: %s\n","How is dwarf fortress going?");
	printf("read:  %s\n",s);

	//TEST: w/r short
	buf_write_short(&b,512);
	d = buf_read_short(&b);
	printf("write: %d\n",512);
	printf("read:  %d\n",d);

	//TEST: w/r long
	buf_write_long(&b,123456789);
	d = buf_read_long(&b);
	printf("write: %d\n",123456789);
	printf("read:  %d\n",d);

	//TEST: w/r float
	buf_write_float(&b,13.37);
	f[0] = buf_read_float(&b);
	printf("write: %f\n",13.37);
	printf("read:  %f\n",f[0]);

	//TEST: w/r float2
	f[0] = 1234.5678;
	f[1] = 2345.6789;
	printf("write: (%f, %f)\n",f[0],f[1]);
	buf_write_float2(&b,f);
	buf_read_float2(&b,f2);
	printf("read:  (%f, %f)\n",f2[0],f2[1]);

	//TEST: w/r float3
	f[0] = 1234.5678;
	f[1] = 2345.6789;
	f[2] = 2345.6789;
	printf("write: (%f, %f, %f)\n",f[0],f[1],f[2]);
	buf_write_float3(&b,f);
	buf_read_float3(&b,f2);
	printf("read:  (%f, %f, %f)\n",f2[0],f2[1],f2[2]);

	return 0;
}
