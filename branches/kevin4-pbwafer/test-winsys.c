#include <stdio.h>
#include "SDL.h"

#include "vid.h"
#include "view.h"
#include "render.h"
#include "menu.h"
#include "input.h"
#include "list.h"

view_t *v1, *v2, *v3;

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
	v1 = VIEW_init(0,0,500,280,"./Font_default.ttf",14,vm.screen,7);
	v2 = VIEW_init(0,280,500,200,"./Font.ttf",7,vm.screen,7);
	v3 = VIEW_init(500,0,200,480,"./Font_default.ttf",10,vm.screen,7);
	menu_view = v3;

	R_color(0,0,0,0,0,255);

	char *bob = "bob";
	// setup global commands
	cmd_t g_cmds[2] = 
	{
		{{KM_NONE,K_c},clearmain,NULL},
		{{KM_NONE,K_p},chatprint,(void *)bob}
	};

	// render the menu
	R_set(v3);
	menu_t *menu,*tmenu;
	menu = MENU_init("Main Menu");
	tmenu = menu;
		menu = MENU_add_menu(menu,tmenu,"Clear View",KM_NONE,K_v);
			menu = MENU_add_entry(menu,tmenu,"Main View",KM_NONE,K_m,clearview,v1);
			menu = MENU_add_entry(menu->par,tmenu,"Chat View",KM_NONE,K_h,clearview,v2);
		menu = MENU_add_menu(menu->par->par,tmenu,"Generate Galaxy",KM_NONE,K_g);
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

	R_set(v2);
	for(i=0;i<45;i++)
		R_addch(' ');
	R_string("dfish - chat window");
	R_update();


	// Input Loop
	cmd_t *c = NULL;
	int ret;
	while((ret = input_handle()))
	{
		if(ret == 2) {
			// Check the global commands
			c = CMD_find_key_array(g_cmds,&g_chain,2);
			if(c != NULL) {
				CMD_do(c);
			}

			// Check the menu commands
			MENU_do_key(menu,&g_chain);
		}
	}

	return 0;
}
