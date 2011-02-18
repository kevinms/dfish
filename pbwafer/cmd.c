#include "cmd.h"

int CMD_diff_key(cmd_t *c, input_t *in)
{
	return input_diff(&(c->i), in);
}

cmd_t *CMD_find_key(list_t *l, input_t *in)
{
	link_t *tmp;

	for(tmp = l->head; tmp != NULL; tmp = tmp->next)
		if(!CMD_diff_key((cmd_t *)tmp->item,in))
			return (cmd_t *)tmp;
	return NULL;
}

cmd_t *CMD_find_key_array(cmd_t c[], input_t *in,int len)
{
	int i;
	for(i = 0; i < len; i++){
		if(!input_diff(&(c[i].i),in)) {
			//fprintf(stderr,"test: %d %d\n",c[i].i.sym, c[i].i.mod);
			return &c[i];
		}
	}
	return NULL;
}

void CMD_do(cmd_t *c)
{
	if(!c->callback)
		return;

	if(c->data) {
		c->callback(c->data);
	}
	else if(c->data_type == 1) {
		c->callback(c->n);
	}
	else
		c->callback();
}

void CMD_do_data(cmd_t *c, void *data)
{
	if(!c->callback || !data)
		return;

	c->callback(data);
}
