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

#include "view_fabricator.h"

#include <stdio.h>

#include "pbwafer/render.h"
#include "pbwafer/input.h"
#include "pbwafer/console.h"
#include "pbwafer/cmd.h"
#include "pbwafer/net.h"
#include "pbwafer/proto.h"

vfab_t *vfab;

cmd_t g_fabcmds[0] = 
{
	//{{KM_NONE,K_i},CB_req_servinfo,NULL},
	//{{KM_NONE,K_c},CB_req_connect,NULL},
};

int id;

void VFAB_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl)
{
	vfab = (vfab_t *)malloc(sizeof(*vfab));
	vfab->v = VIEW_init(x,y,real_w,real_h,fontname,fontsize,screen,numl);

	vfab->v->accept_input = VFAB_accept_input;
	vfab->v->showcursor = 1;
}

void VFAB_menu_init(menu_t *r, int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl)
{
	menu_t *menu;

	// Build the space view menu
	menu = r;
		menu = MENU_add_menu(menu,r,"Fabricator",KM_NONE,K_f);
		menu->c.callback = CB_fabricator;

			menu = MENU_add_entry(menu,r,"New Blueprint",KM_NONE,K_n,CB_new_bp,NULL);
			menu = MENU_add_menu(menu->par,r,"Edit Blueprint",KM_NONE,K_b);
			menu = MENU_add_entry_int(menu,r,"wall",KM_NONE,K_a,CB_add_entity,0);
			menu = MENU_add_entry_int(menu->par,r,"floor",KM_NONE,K_b,CB_add_entity,1);
}

void VFAB_accept_input()
{
	cmd_t *c = NULL;

	R_set(vfab->v);

	// Check the menu commands
	c = CMD_find_key_array(g_fabcmds,&g_chain,0);
	if(c != NULL)
		CMD_do(c);
	MENU_do_key(vfab->menu,&g_chain);

	if(g_chain.sym == K_UP) {
		R_moveby(0,-1);
	}
	else if(g_chain.sym == K_DOWN) {
		R_moveby(0,1);
	}
	else if(g_chain.sym == K_LEFT) {
		R_moveby(-1,0);
	}
	else if(g_chain.sym == K_RIGHT) {
		R_moveby(1,0);
	}

	VFAB_update();
}

void VFAB_update()
{
	R_set(vfab->v);
	R_clear();

	//R_movetocenter();
	//R_blueprint(vfab->g_bp_edit);
	if(vfab->g_bp_edit) {
		BP_render(vfab->g_bp_edit);
	}
	//TODO: draw any blueprints,planets,objects that are in the view

	R_update();
}

void CB_fabricator(void *data)
{
	// Set the focus to the space view
	VIEW_focus(vfab->v);

	vfab->v->showcursor = 0;
	VFAB_update();

	MENU_load((menu_t *)data);
}

void CB_new_bp()
{
	R_movetocenter();
	vfab->v->showcursor = 1;
	vfab->g_bp_edit = BP_init();
	vfab->g_bp_edit->x = vfab->v->cursor_x;
	vfab->g_bp_edit->y = vfab->v->cursor_y;
}

void CB_add_entity(int n)
{
	CONSOLE_print("Add entity: %d",n);
	BP_add_entity(vfab->g_bp_edit,n, vfab->v->cursor_x, vfab->v->cursor_y);
}
