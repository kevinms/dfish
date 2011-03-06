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

#ifndef __VIEW_H
#define __VIEW_H

#include "SDL_ttf.h"

//TODO: add a variable for setting which view has focus
//TODO: get view resizing working so you can do things like hide the menu view

//TODO: Need some type of configuration file.  This is so the user can customize
//      all the views to their liking (size, location)

typedef struct view_s
{
	int real_w; // Root SDL_Surface width for this view in pixels
	int real_h; // Root SDL_Surface height for this view in pixels

	int tile_w; // width of a single tile
	int tile_h; // height of a single tile

	int fake_w; // width by # of tiles
	int fake_h; // height by # of tiles

	int x; // x position of the view on the ROOT WINDOW SDL_Surface
	int y; // y position of the view on the ROOT WINDOW SDL_Surface

	int cursor_x; // the current x position of the cursor in this view
	int cursor_y; // the current y position of the cursor in this view

	char *fontname;
	int fontsize;
	TTF_Font *font;

	char cursor;
	char showcursor;
	char cursor_color[4];
	int numl; // Number of layers that exist
	char *layer_visible; // Array of bools to set whether to render a layer
	int l; // The current layer to render things on

	SDL_Surface *screen;
	Uint32 ****layers;

	// Callback functions
	void (*accept_input)();
	void (*update)();
} view_t;

extern view_t *active_view;
extern view_t *prev_view;

view_t *VIEW_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);
void VIEW_layers_malloc(view_t *v);
void VIEW_resize(view_t *v);
void VIEW_update(view_t *v);

void VIEW_focus(view_t *view);

void VIEW_free(view_t *v);

#endif /* !__VIEW_H */
