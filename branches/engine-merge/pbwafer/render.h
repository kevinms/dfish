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

#ifndef __RENDER_H
#define __RENDER_H

#include "view.h"
#include "blueprint.h"

#define CLAMP(x) if((x) < 0) (x) = 0; else if((x) > 255) (x) = 255;

//TODO: Implement robust blueprint rendering

// Initialize render module and set view
void R_init(void);
void R_set(view_t *v);

// Cursor rendering options
void R_setcursorlook(char r,char g,char b);
void R_showcursor(char b);

// Basic render functions
void R_setch(int ch);
void R_addch(int ch);
void R_string(const char *s);
void R_stringln(const char *s);
void R_stringn(const char *s,int n);
void R_stringnln(const char *s,int n);
//void R_blueprint(bp_t *b);
void R_move(int x,int y);
void R_movetocenter();
void R_moveby(int x,int y);
void R_updatelayer(int l);
void R_update(void);
void R_updateall(void);
void R_clearlayer(int l);
void R_clear(void);
void R_color(char r,char g,char b,char fr,char fg,char fb);

// Change color then render
void R_csetch(int ch,char r,char g,char b,char fr,char fg,char fb);
void R_caddch(int ch,char r,char g,char b,char fr,char fg,char fb);
void R_cstring(const char *s,char r,char g,char b,char fr,char fg,char fb);

// Move cursor then render
void R_mvsetch(int ch,int x,int y);
void R_mvaddch(int ch,int x,int y);
void R_mvstring(const char *s,int x,int y);
void R_mvclearch(int l,int x,int y);

// Move and change color then render
void R_setchext(int ch,int x,int y,char r,char g,char b,char fr,char fg,char fb);
void R_addchext(int ch,int x,int y,char r,char g,char b,char fr,char fg,char fb);
void R_stringext(const char *s,int x,int y,char r,char g,char b,char fr,char fg,char fb);
void R_clearlayerext(int ch,char r,char g,char b,char fr,char fg,char fb, int l);
void R_clearext(int ch,char r,char g,char b,char fr,char fg,char fb);

// Advanced functions
void R_setalpha(char a);
void R_setlayer(char l);
void R_setlayershown(char l,char s);
void R_setcolor(int x,int y,char r,char g,char b,char fr,char fg,char fb);
void R_addcolor(int x,int y,int r,int g,int b,int fr,int fg,int fb);
void R_box(view_t *view);
void R_boxext(view_t *view,char r,char g,char b);

#endif /* !__RENDER_H */
