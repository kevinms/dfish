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
				menu = MENU_add_menu(menu,r,"Walls",KM_NONE,K_w);
					menu = MENU_add_entry_int(menu,r,"Wood",KM_NONE,K_a,CB_add_entity,1);
					menu = MENU_add_entry_int(menu->par,r,"Steel",KM_NONE,K_b,CB_add_entity,2);
					menu = MENU_add_entry_int(menu->par,r,"Adamantium",KM_NONE,K_c,CB_add_entity,3);
				menu = MENU_add_menu(menu->par->par,r,"Floors",KM_NONE,K_f);
					menu = MENU_add_entry_int(menu,r,"Wood",KM_NONE,K_a,CB_add_entity,51);
					menu = MENU_add_entry_int(menu->par,r,"Steel",KM_NONE,K_b,CB_add_entity,52);
					menu = MENU_add_entry_int(menu->par,r,"Adamantium",KM_NONE,K_c,CB_add_entity,53);
}

void VFAB_accept_input()
{
	cmd_t *c = NULL;

	// Check the menu commands
	c = CMD_find_key_array(g_fabcmds,&g_chain,0);
	if(c != NULL)
		CMD_do(c);
	MENU_do_key(vfab->menu,&g_chain);

	if(g_chain.sym == K_UP) {
		R_moveby(0,-1);
		vfab->cursor_y--;
	}
	else if(g_chain.sym == K_DOWN) {
		R_moveby(0,1);
		vfab->cursor_y++;
	}
	else if(g_chain.sym == K_LEFT) {
		R_moveby(-1,0);
		vfab->cursor_x--;
	}
	else if(g_chain.sym == K_RIGHT) {
		R_moveby(1,0);
		vfab->cursor_x++;
	}

	VFAB_update();
}

void VFAB_update()
{
	R_set(vfab->v);
	R_clear();

	R_movetocenter();
	//R_blueprint(vfab->g_bp_edit);
	//TODO: draw any blueprints,planets,objects that are in the view

	R_update();
}

void CB_fabricator(void *data)
{
	// Set the focus to the space view
	VIEW_focus(vfab->v);

	MENU_load((menu_t *)data);
}

void CB_new_bp()
{
	vfab->g_bp_edit = (bp_t *)malloc(sizeof(*vfab->g_bp_edit));
}

void CB_add_entity(int n)
{
	CONSOLE_print("Add entity: %d",n);
	BP_add_entity(vfab->g_bp_edit,n);

	VFAB_update();
}
