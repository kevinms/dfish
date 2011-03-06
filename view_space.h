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

#ifndef __VIEW_SPACE_H
#define __VIEW_SPACE_H

#include "pbwafer/menu.h"
#include "pbwafer/view.h"

typedef struct vspace_s {
	view_t *v;
	view_t *v_menu;
	menu_t *menu;
} vspace_t;

extern vspace_t *vspace;

void VSPACE_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);
void VSPACE_menu_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);
void VSPACE_update();
void VSPACE_accept_input();

// Space view callbacks
void CB_req_servinfo_bcast();
void CB_req_servinfo();
void CB_req_connect();
void CB_console_fuzz();
void CB_gengalaxy();

#endif /* !__VIEW_SPACE_H */
