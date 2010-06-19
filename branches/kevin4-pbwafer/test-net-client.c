#include <stdio.h>
#include "SDL.h"

#include "vid.h"
#include "view.h"
#include "render.h"
#include "menu.h"
#include "input.h"
#include "list.h"
#include "console.h"
#include "buf.h"
#include "cmd.h"
#include "net.h"
#include "proto.h"

net_t *netserv;

menu_t *menu,*tmenu;
view_t *v1, *v2, *v3;

void console_fuzz()
{
	int i;
	for(i = 0; i < 10; i++)
		CONSOLE_print("SEGFAULT DANG IT!!");
}

void req_servinfo()
{
	PROTO_req_servinfo_ip("127.0.0.1", "8888");
	printf("hi\n");
}

void req_connect()
{
	netserv = NET_socket_client("127.0.0.1","8888");
	PROTO_connect("127.0.0.1", "8888");
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

cmd_t g_maincmds[2] = 
{
	{{KM_NONE,K_i},req_servinfo,NULL},
	{{KM_NONE,K_c},req_connect,NULL},
};

void mainview_accept_input()
{
	cmd_t *c = NULL;
	// Check the menu commands
	c = CMD_find_key_array(g_maincmds,&g_chain,2);
	if(c != NULL)
		CMD_do(c);

	MENU_do_key(menu,&g_chain);
}

int main(void)
{
	// initialize the video system
	vidmode_t vm;
	vm.width = 700;
	vm.height = 480;
	vm.bpp = 32;
	VID_init();
	VID_init_mode(&vm);
	VID_set_caption("pbwafer - test driver");

	atexit(SDL_Quit);

	// initilize systems
	R_init();
	input_init();
	NET_init();
	PROTO_init();

	// initilize 2 different views
	v1 = VIEW_init(0,0,500,280,"./Font.ttf",7,vm.screen,7);
	v2 = VIEW_init(0,280,500,200,"./Font_default.ttf",10,vm.screen,7);
	v3 = VIEW_init(500,0,200,480,"./Font_default.ttf",10,vm.screen,7);
	menu_view = v3;
	VIEW_focus(v1);
	//active_view = v2;

	v1->accept_input = mainview_accept_input;

	R_color(0,0,0,0,0,255);

	// setup global commands
	cmd_t g_cmds[1] = 
	{
		{{KM_NONE,K_BACKQUOTE},CONSOLE_toggle,NULL}
	};

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
	//int i;
	//for (i = 0; i < 80*15; i++)
		//R_addch(0x00AB);
	R_update();

	//R_set(v3);

	CONSOLE_init("PBwafer Console",PROTO_client_send_chat);
	CONSOLE_attach_to_view(v2);
	CONSOLE_print("SUCCESS: This successfully printed to the console");

	CONSOLE_print("----------Console Guide----------");
	CONSOLE_print("` key will toggle between the main view and console view");
	CONSOLE_print("Any printable key can be typed into the console");
	CONSOLE_print("Up Arrow Key will go back in the history of commands you typed");
	CONSOLE_print("Down Arrow Key will go forward in the history of commands you typed");
	CONSOLE_print("Backspace will delete the most recent typed character");
	CONSOLE_print("Page Up will scroll up one page of lines (All the junk that was printed to the console and should wrap around)");
	CONSOLE_print("Page Down will scroll down one page of lines");
	CONSOLE_print("%d",5);

	//CONSOLE_update();

	R_set(v3);
	R_update();

	PROTO_set_clientinfo(STATE_NOP,"pughar");

	// Input Loop
	cmd_t *c = NULL;
	int ret;

	for(;;) {
		if(PROTO_recv() > 0)
			PROTO_client_parse_DGRAM();

		ret = input_handle();
			if(ret == 2) {
				// Check the global commands
				c = CMD_find_key_array(g_cmds,&g_chain,1);
				if(c != NULL) {
					CMD_do(c);
				} else {
					active_view->accept_input();
				}
			} else if(ret == 0){
				return 0;
			}
	}

	return 0;
}
