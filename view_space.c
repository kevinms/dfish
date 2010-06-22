#include "view_space.h"

#include <stdio.h>

#include "pbwafer/render.h"
#include "pbwafer/input.h"
#include "pbwafer/console.h"
#include "pbwafer/cmd.h"
#include "pbwafer/net.h"
#include "pbwafer/proto.h"

vspace_t *vspace;

cmd_t g_spacecmds[2] = 
{
	{{KM_NONE,K_i},CB_req_servinfo,NULL},
	{{KM_NONE,K_c},CB_req_connect,NULL},
};

void VSPACE_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl)
{
	vspace = (vspace_t *)malloc(sizeof(*vspace));
	vspace->v = VIEW_init(x,y,real_w,real_h,fontname,fontsize,screen,numl);

	vspace->v->accept_input = VSPACE_accept_input;

	// Set the focus to the space view
	VIEW_focus(vspace->v);

	// Print welcome message
	R_color(0,0,0,0,0,255);
	R_set(vspace->v);
	R_stringln("Welcome to dfish!  The first ever online multiplayer space demolition derby!");
	R_update();
}

void VSPACE_menu_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl)
{
	menu_t *menu;
	menu_t *tmenu;

	vspace->v_menu = VIEW_init(x,y,real_w,real_h,fontname,fontsize,screen,numl);

	// What view to always render menus to
	menu_view = vspace->v_menu;

	// Build the space view menu
	menu = MENU_init("Main Menu");
	tmenu = menu;
		menu = MENU_add_menu(menu,tmenu,"Network",KM_NONE,K_v);
			menu = MENU_add_entry(menu,tmenu,"Find LAN Servers",KM_NONE,K_m,CB_req_servinfo_bcast,NULL);
			menu = MENU_add_entry(menu->par,tmenu,"Connect to lo",KM_NONE,K_h,CB_req_connect,NULL);
		menu = MENU_add_menu(menu->par->par,tmenu,"Generate Galaxy",KM_NONE,K_g);
			menu = MENU_add_entry(menu,tmenu,"New Galaxy",KM_NONE,K_m,CB_gengalaxy,NULL);
		menu = MENU_add_entry(menu->par->par,tmenu,"Fuzz!!!",KM_NONE,K_p,CB_console_fuzz,NULL);

	// Render the menu
	R_color(0,0,0,0,0,255);
	MENU_load(tmenu);
	tmenu->cur = tmenu;
	vspace->menu = tmenu;

	// Render the current menu (Redundant, yes?)
	R_set(vspace->v_menu);
	R_update();
}

void VSPACE_accept_input()
{
	cmd_t *c = NULL;

	// Check the menu commands
	c = CMD_find_key_array(g_spacecmds,&g_chain,2);
	if(c != NULL)
		CMD_do(c);

	MENU_do_key(vspace->menu,&g_chain);
}

void CB_req_servinfo_bcast()
{
	PROTO_req_servinfo_broadcast("8888");
}

void CB_req_servinfo()
{
	PROTO_req_servinfo_ip("127.0.0.1", "8888");
}

void CB_req_connect()
{
	NET_socket_client("127.0.0.1","8888");
	PROTO_connect("127.0.0.1", "8888");
}

void CB_console_fuzz()
{
	int i;
	for(i = 0; i < 10; i++)
		CONSOLE_print("SEGFAULT DANG IT!!");
}

void CB_gengalaxy()
{
	CONSOLE_print("ERROR: this is not implemented yet");
}
