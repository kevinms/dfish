#include <stdio.h>
#include "SDL.h"
#include "input.h"

int main (int argc, char *argv[]) {

	SDL_Surface *screen;
	 
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	 
	atexit(SDL_Quit);
	 
	screen = SDL_SetVideoMode(320, 240, 0, SDL_ANYFORMAT);
	if (screen == NULL) {
		printf("Unable to set video mode: %s\n", SDL_GetError());
		return 1;
	}

	SDL_WM_SetCaption( "Dfish", NULL );

	input_init();

	while(input_handle()) {
		
	}

	SDL_Quit();
	return 0;
}
