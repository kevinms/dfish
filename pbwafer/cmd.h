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
