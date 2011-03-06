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

#ifndef __VID_H
#define __VID_H

#include "SDL.h"

typedef struct vidmode_s
{
	int width;
	int height;
	int bpp;
	SDL_Surface *screen;
} vidmode_t;

void VID_init(void);
void VID_shutdown(void);
vidmode_t *VID_init_mode(int w,int h,int bpp,const char *s);
void VID_set_caption(const char *name);
void VID_new_screen();

#endif /* !__VID_H */
