#include <stdio.h>
#include "SDL.h"

#include "vid.h"
#include "view.h"
#include "render.h"

int main(void)
{
	// initialize the video system
	vidmode_t vm;
	vm.width = 640;
	vm.height = 480;
	vm.bpp = 32;
	VID_init();
	VID_init_mode(&vm);
	VID_set_caption("pbwafer");

	atexit(SDL_Quit);

	// initilize render system
	R_init();

	// initilize 2 different views
	view_t *v1, *v2;
	v1 = VIEW_init(0,0,640,280,"./Font_default.ttf",14,vm.screen);
	v2 = VIEW_init(0,280,640,200,"./Font.ttf",7,vm.screen);

	// render a boarder for each view

	// render text in both views
	R_set(v1);
	R_char('P');
	R_char('B');
	R_char('W');
	R_char('A');
	R_char('F');
	R_char('E');
	R_char('R');
	R_string(" rocks!");
	int i;
	for (i = 0; i < 80*15; i++)
		R_char(0x00AB);
	R_update();

	R_set(v2);
	for(i=0;i<45;i++)
		R_char(' ');
	R_string("dfish - chat window");
	R_update();

	SDL_Event event;
	int running = 1;

	while(running) {
		while(SDL_PollEvent(&event)) {
			switch(event.type){
				case SDL_KEYDOWN:
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
