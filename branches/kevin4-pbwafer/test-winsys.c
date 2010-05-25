#include <stdio.h>
#include "SDL.h"

#include "vid.h"
#include "view.h"
#include "render.h"
#include "menu.h"

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

	// initilize 2 different views
	view_t *v1, *v2, *v3;
	v1 = VIEW_init(0,0,500,280,"./Font_default.ttf",14,vm.screen,7);
	v2 = VIEW_init(0,280,500,200,"./Font.ttf",7,vm.screen,7);
	v3 = VIEW_init(500,0,200,480,"./Font_default.ttf",10,vm.screen,7);

	R_color(0,0,0,0,0,255);

	// render the menu
	R_set(v3);
	menu_t *menu;
	keychain_t kc = {'b','l'};
	menu = MENU_init("Main Menu");
		menu = MENU_add_entry(menu,"Clear View",kc,NULL);
			menu = MENU_add_entry(menu,"Main View",kc,NULL);
			menu = MENU_add_entry(menu->par,"Chat View",kc,NULL);
		menu = MENU_add_entry(menu->par->par,"Generate Galaxy",kc,NULL);
	MENU_load(menu->par);
	R_update();

	// render text in both views
	R_set(v1);
	R_addch('P');
	R_addch('B');
	R_addch('W');
	R_addch('A');
	R_addch('F');
	R_addch('E');
	R_addch('R');
	R_string(" rocks!");
	int i;
	for (i = 0; i < 80*15; i++)
		R_addch(0x00AB);
	R_update();

	R_set(v2);
	for(i=0;i<45;i++)
		R_addch(' ');
	R_string("dfish - chat window");
	R_update();

	SDL_Event event;
	int running = 1;

	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type){
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == 'c') {
						R_set(v1);
						R_clear();
						R_update();
					}
				case SDL_KEYUP:
					break;
				case SDL_QUIT:
					running = 0;
					break;
			}
		}
	}

	return 0;
}
