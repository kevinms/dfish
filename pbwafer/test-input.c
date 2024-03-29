// Copyright (C) 2011  Kevin Smith
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

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
