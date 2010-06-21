#include <stdio.h>
#include "vid.h"
#include "SDL.h"

int main(void)
{
	vidmode_t vm;
	vm.width = 640;
	vm.height = 480;
	vm.bpp = 32;
	VID_init();
	VID_init_mode(&vm);
	VID_set_caption("pbwafer");

	atexit(SDL_Quit);

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
