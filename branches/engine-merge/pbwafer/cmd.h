#ifndef __CMD_H
#define __CMD_H

#include "list.h"
#include "input.h"

typedef struct cmd_s
{
	input_t i;
	void (*callback)();
	void *data; // pointer to pass to callback
	int data_type;
	int n;      // int to pass to callback
} cmd_t;

int CMD_compare_key(cmd_t *c, input_t *in);
cmd_t *CMD_find_key(list_t *l, input_t *in);
cmd_t *CMD_find_key_array(cmd_t c[], input_t *in,int len);
void CMD_do(cmd_t *c);
void CMD_do_data(cmd_t *c, void *data);

#endif /* !__CMD_H */
