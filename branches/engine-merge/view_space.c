#include "view-space.h"

cmd_t g_maincmds[2] = 
{
	{{KM_NONE,K_i},req_servinfo,NULL},
	{{KM_NONE,K_c},req_connect,NULL},
};

void console_fuzz()
{
	int i;
	for(i = 0; i < 10; i++)
		CONSOLE_print("SEGFAULT DANG IT!!");
}

void gengalaxy()
{
	CONSOLE_print("ERROR: this is not implemented yet");
}

void clearview(void *data)
{
	R_set((view_t *)data);
	R_clear();
	R_update();
}

void clearmain()
{
	R_set(v1);
	R_clear();
	R_update();
}

void mainview_accept_input()
{
	cmd_t *c = NULL;
	// Check the menu commands
	c = CMD_find_key_array(g_maincmds,&g_chain,2);
	if(c != NULL)
		CMD_do(c);

	MENU_do_key(menu,&g_chain);
}

view_t *VSPACE_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl)
{
	view_t *vnew;
	vnew = VIEW_init(x,y,real_w,real_h,fontname,fontsize,screen,numl);

	// render text in both views
	R_set(v1);
	R_addch('P');
	R_stringln("BWAFER rocks!");
	R_stringln("");
	R_stringln("Run both test-net-server and test-net client in seperate terminals");
	R_stringln("");
	R_stringln("A few things you can try:");
	R_stringln("'i'  will query the server for information");
	R_stringln("     this information will be printed in your terminal");
	R_stringln("'c'  will try to connect to the server");
	R_update();
}

view_t *VSPACE_menu_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl)
{
	view_t *vnew;
	vnew = VIEW_init(x,y,real_w,real_h,fontname,fontsize,screen,numl);

	R_color(0,0,0,0,0,255);

	// render the menu
	R_set(v3);
	menu = MENU_init("Main Menu");
	tmenu = menu;
		menu = MENU_add_menu(menu,tmenu,"Clear View",KM_NONE,K_v);
			menu = MENU_add_entry(menu,tmenu,"Main View",KM_NONE,K_m,clearview,v1);
			menu = MENU_add_entry(menu->par,tmenu,"Chat View",KM_NONE,K_h,clearview,v2);
		menu = MENU_add_menu(menu->par->par,tmenu,"Generate Galaxy",KM_NONE,K_g);
			menu = MENU_add_entry(menu,tmenu,"New Galaxy",KM_NONE,K_m,gengalaxy,v1);
		menu = MENU_add_entry(menu->par->par,tmenu,"Fuzz!!!",KM_NONE,K_p,console_fuzz,v1);
	MENU_load(tmenu);
	tmenu->cur = tmenu;
	menu = tmenu;
	R_update();
}
