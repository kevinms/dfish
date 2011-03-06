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

#ifndef __VIEW_FABRICATOR_H
#define __VIEW_FABRICATOR_H

#include "pbwafer/menu.h"
#include "pbwafer/view.h"
#include "pbwafer/blueprint.h"

typedef struct vfab_s {
	view_t *v;
	view_t *v_menu;
	menu_t *menu;

	int cursor_x;
	int cursor_y;

	bp_t *g_bp_edit;

	
} vfab_t;

extern vfab_t *vfab;

void VFAB_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);
void VFAB_menu_init(menu_t *r, int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);
void VFAB_update();
void VFAB_accept_input();

void CB_fabricator(void *data);
void CB_new_bp();
void CB_add_entity(int n);

#endif /* !__VIEW_FABRICATOR_H */
