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

#ifndef __MENU_H
#define __MENU_H

#include "list.h"

#include "input.h"
#include "cmd.h"
#include "view.h"

//TODO: Things that need to be handled by the menu system:
//          - Menu name or entry name is too long to fit in the view...
//          - There are too many entries to fit in the view vertically...
//          - Not all menu's may want to have a cancel option...

//TODO: Cleanup unused functions

typedef struct menu_s
{
	int id;
	struct menu_s *par;
	const char *name;
	list_t *entry_list;
	struct menu_s *cur;

	struct menu_s *cancel;

	cmd_t c;
	struct menu_s *root;
} menu_t;

extern view_t *menu_view;

menu_t *MENU_init(const char *name);
menu_t *MENU_add_menu(menu_t *par,menu_t *root,const char *name,int mod, int sym);
menu_t *MENU_init_entry(menu_t *par,menu_t *root,const char *name,int mod,int sym,void (*callback)(),void *data);
menu_t *MENU_add_entry(menu_t *par,menu_t *root,const char *name,int mod,int sym,void (*callback)(),void *data);

menu_t *MENU_init_entry_int(menu_t *par,menu_t *root,const char *name,int mod,int sym,void (*callback)(),int n);
menu_t *MENU_add_entry_int(menu_t *par,menu_t *root,const char *name,int mod,int sym,void (*callback)(),int n);

menu_t *MENU_do_key(menu_t *root, input_t *in);
void MENU_load(menu_t *m);
void MENU_free(menu_t *m);

void MENU_do_load_menu(void *data);
void MENU_do_load_par(void *data);

#endif /* !__MENU_H */
