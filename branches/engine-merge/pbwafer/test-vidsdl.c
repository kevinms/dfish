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
