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
