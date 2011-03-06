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

#include "vid.h"

void VID_init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		fprintf(stderr,"SDL Video subsystem failed to initialize: %s\n",SDL_GetError());

	atexit(SDL_Quit);
}

vidmode_t *VID_init_mode(int w,int h,int bpp,const char *s)
{
	vidmode_t *mode;
	mode = (vidmode_t *)malloc(sizeof(*mode));
	mode->width = w;
	mode->height = h;
	mode->bpp = bpp;

	// The real width and height of the window is provided here.  This is the 
	// root screen.  All other screen's will be fast blitted onto this one.
	if((mode->screen = SDL_SetVideoMode(mode->width,mode->height,mode->bpp,SDL_SWSURFACE)) == NULL)
		fprintf(stderr,"Video initialization failed: %s\n",SDL_GetError());

	VID_set_caption(s);

	return mode;
}

void VID_set_caption(const char *name)
{
	SDL_WM_SetCaption(name, NULL);
}

void VID_shutdown()
{
	SDL_Quit();
}
