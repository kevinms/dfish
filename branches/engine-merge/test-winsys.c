#include <stdio.h>
#include "SDL.h"

#include "vid.h"
#include "view.h"
#include "render.h"
#include "menu.h"
#include "input.h"
#include "list.h"
#include "console.h"

menu_t *menu,*tmenu;
view_t *v1, *v2, *v3;

void mainview_accept_input()
{
	// Check the menu commands
	MENU_do_key(menu,&g_chain);
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

void chatprint(void *s)
{
	printf("%s\n",(char *)s);
	R_set(v2);
	R_string((char *)s);
	R_update();
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

	// initilize render system
	R_init();
	input_init();

	// initilize 2 different views
	v1 = VIEW_init(0,0,500,280,"./Font.ttf",7,vm.screen,7);
	v2 = VIEW_init(0,280,500,200,"./Font_default.ttf",10,vm.screen,7);
	v3 = VIEW_init(500,0,200,480,"./Font_default.ttf",10,vm.screen,7);
	menu_view = v3;
	VIEW_focus(v1);
	//active_view = v2;

	v1->accept_input = mainview_accept_input;

	R_color(0,0,0,0,0,255);

	char *bob = "bob";
	// setup global commands
	cmd_t g_cmds[3] = 
	{
		{{KM_NONE,K_c},clearmain,NULL},
		{{KM_NONE,K_p},chatprint,(void *)bob},
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
	MENU_load(tmenu);
	tmenu->cur = tmenu;
	menu = tmenu;
	R_update();

	// render text in both views
	R_set(v1);
	R_addch('P');
	R_string("BWAFER rocks!");
	int i;
	for (i = 0; i < 80*15; i++)
		R_addch(0x00AB);
	R_update();

	//R_set(v3);

	CONSOLE_init("PBwafer Console");
	CONSOLE_attach_to_view(v2);
	CONSOLE_print("SUCCESS: This successfully printed to the console");

	CONSOLE_print("----------Console Guide----------");
	CONSOLE_print("` key will toggle between the main view and console view");
	CONSOLE_print("Any printable key can be typed into the console");
	CONSOLE_print("Up Arrow Key will go back in the history of commands you typed");
	CONSOLE_print("Down Arrow Key will go forward in the history of commands you typed");
	CONSOLE_print("Backspace will delete the most recent typed character");
	CONSOLE_print("Page Up will scroll up one page of lines (All the junk that was printed to the console and should wrap around");
	CONSOLE_print("Page Down will scroll down one page of lines");
	CONSOLE_print("1");
	CONSOLE_print("2");
	CONSOLE_print("3");
	CONSOLE_print("4");
	CONSOLE_print("5");
	CONSOLE_print("6");
	CONSOLE_print("7");
	CONSOLE_print("8");
	CONSOLE_print("9");
	CONSOLE_print("10");
	CONSOLE_print("11");
	CONSOLE_print("12");
	CONSOLE_print("13");
	CONSOLE_print("14");
	CONSOLE_print("15");
	CONSOLE_print("16");
	CONSOLE_print("17");
	CONSOLE_print("18");
	CONSOLE_print("19");
	CONSOLE_print("20");
	CONSOLE_print("%d",5);

	//CONSOLE_update();

	R_set(v3);
	R_update();

	// Input Loop
	cmd_t *c = NULL;
	int ret;
	while((ret = input_handle()))
	{
		if(ret == 2) {
			// Check the global commands
			c = CMD_find_key_array(g_cmds,&g_chain,3);
			if(c != NULL) {
				CMD_do(c);
			} else {
				active_view->accept_input();
			}
		}
	}

	return 0;
}
