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

#ifndef __INPUT_H
#define __INPUT_H

//TODO: Goals for the input system:
//  - chainable key event system
//  - able to map keys to whatever commands the user wants
//  - some mouse support as well
//  - something similar to the circle selection in blender
//  - four to six keys can be pressed
//  - chainable key event system could use some

//TODO: Begin using SDL_GetKeyState() to find ALL keystates (needed anymore?)

#include "SDL.h"

#define K_MOUSE1 1337
#define K_MOUSE2 1338
#define K_MOUSE3 1339
#define K_WHEELUP 1336
#define K_WHEELDOWN 1335

#define K_BACKSPACE    SDLK_BACKSPACE
#define K_TAB          SDLK_TAB
#define K_CLEAR        SDLK_CLEAR
#define K_RETURN       SDLK_RETURN
#define K_PAUSE        SDLK_PAUSE
#define K_ESCAPE       SDLK_ESCAPE
#define K_SPACE        SDLK_SPACE
#define K_EXCLAIM      SDLK_EXCLAIM
#define K_QUOTEDBL     SDLK_QUOTEDBL
#define K_HASH         SDLK_HASH
#define K_DOLLAR       SDLK_DOLLAR
#define K_AMPERSAND    SDLK_AMPERSAND
#define K_QUOTE        SDLK_QUOTE
#define K_LEFTPAREN    SDLK_LEFTPAREN
#define K_RIGHTPAREN   SDLK_RIGHTPAREN
#define K_ASTERISK     SDLK_ASTERISK
#define K_PLUS         SDLK_PLUS
#define K_COMMA        SDLK_COMMA
#define K_MINUS        SDLK_MINUS
#define K_PERIOD       SDLK_PERIOD
#define K_SLASH        SDLK_SLASH
#define K_0            SDLK_0
#define K_1            SDLK_1
#define K_2            SDLK_2
#define K_3            SDLK_3
#define K_4            SDLK_4
#define K_5            SDLK_5
#define K_6            SDLK_6
#define K_7            SDLK_7
#define K_8            SDLK_8
#define K_9            SDLK_9
#define K_COLON        SDLK_COLON
#define K_SEMICOLON    SDLK_SEMICOLON
#define K_LESS         SDLK_LESS
#define K_EQUALS       SDLK_EQUALS
#define K_GREATER      SDLK_GREATER
#define K_QUESTION     SDLK_QUESTION
#define K_AT           SDLK_AT
#define K_LEFTBRACKET  SDLK_LEFTBRACKET
#define K_BACKSLASH    SDLK_BACKSLASH
#define K_RIGHTBRACKET SDLK_RIGHTBRACKET
#define K_CARET        SDLK_CARET
#define K_UNDERSCORE   SDLK_UNDERSCORE
#define K_BACKQUOTE    SDLK_BACKQUOTE
#define K_a            SDLK_a
#define K_b            SDLK_b
#define K_c            SDLK_c
#define K_d            SDLK_d
#define K_e            SDLK_e
#define K_f            SDLK_f
#define K_g            SDLK_g
#define K_h            SDLK_h
#define K_i            SDLK_i
#define K_j            SDLK_j
#define K_k            SDLK_k
#define K_l            SDLK_l
#define K_m            SDLK_m
#define K_n            SDLK_n
#define K_o            SDLK_o
#define K_p            SDLK_p
#define K_q            SDLK_q
#define K_r            SDLK_r
#define K_s            SDLK_s
#define K_t            SDLK_t
#define K_u            SDLK_u
#define K_v            SDLK_v
#define K_w            SDLK_w
#define K_x            SDLK_x
#define K_y            SDLK_y
#define K_z            SDLK_z
#define K_DELETE       SDLK_DELETE
#define K_KP0          SDLK_KP0
#define K_KP1          SDLK_KP1
#define K_KP2          SDLK_KP2
#define K_KP3          SDLK_KP3
#define K_KP4          SDLK_KP4
#define K_KP5          SDLK_KP5
#define K_KP6          SDLK_KP6
#define K_KP7          SDLK_KP7
#define K_KP8          SDLK_KP8
#define K_KP9          SDLK_KP9
#define K_KP_PERIOD    SDLK_KP_PERIOD
#define K_KP_DIVIDE    SDLK_KP_DIVIDE
#define K_KP_MULTIPLY  SDLK_KP_MULTIPLY
#define K_KP_MINUS     SDLK_KP_MINUS
#define K_KP_PLUS      SDLK_KP_PLUS
#define K_KP_ENTER     SDLK_KP_ENTER
#define K_KP_EQUALS    SDLK_KP_EQUALS
#define K_UP           SDLK_UP
#define K_DOWN         SDLK_DOWN
#define K_RIGHT        SDLK_RIGHT
#define K_LEFT         SDLK_LEFT
#define K_INSERT       SDLK_INSERT
#define K_HOME         SDLK_HOME
#define K_END          SDLK_END
#define K_PAGEUP       SDLK_PAGEUP
#define K_PAGEDOWN     SDLK_PAGEDOWN
#define K_F1           SDLK_F1
#define K_F2           SDLK_F2
#define K_F3           SDLK_F3
#define K_F4           SDLK_F4
#define K_F5           SDLK_F5
#define K_F6           SDLK_F6
#define K_F7           SDLK_F7
#define K_F8           SDLK_F8
#define K_F9           SDLK_F9
#define K_F10          SDLK_F10
#define K_F11          SDLK_F11
#define K_F12          SDLK_F12
#define K_F13          SDLK_F13
#define K_F14          SDLK_F14
#define K_F15          SDLK_F15
#define K_NUMLOCK      SDLK_NUMLOCK
#define K_CAPSLOCK     SDLK_CAPSLOCK
#define K_SCROLLOCK    SDLK_SCROLLOCK
#define K_RSHIFT       SDLK_RSHIFT
#define K_LSHIFT       SDLK_LSHIFT
#define K_RCTRL        SDLK_RCTRL
#define K_LCTRL        SDLK_LCTRL
#define K_RALT         SDLK_RALT
#define K_LALT         SDLK_LALT
#define K_RMETA        SDLK_RMETA
#define K_LMETA        SDLK_LMETA
#define K_LSUPER       SDLK_LSUPER
#define K_RSUPER       SDLK_RSUPER
#define K_MODE         SDLK_MODE
#define K_HELP         SDLK_HELP
#define K_PRINT        SDLK_PRINT
#define K_SYSREQ       SDLK_SYSREQ
#define K_BREAK        SDLK_BREAK
#define K_MENU         SDLK_MENU
#define K_POWER        SDLK_POWER
#define K_EURO         SDLK_EURO

#define KM_NONE        KMOD_NONE
#define KM_NUM         KMOD_NUM
#define KM_CAPS        KMOD_CAPS
#define KM_LCTRL       KMOD_LCTRL
#define KM_RCTRL       KMOD_RCTRL
#define KM_RSHIFT      KMOD_RSHIFT
#define KM_LSHIFT      KMOD_LSHIFT
#define KM_RALT        KMOD_RALT
#define KM_LALT        KMOD_LALT
#define KM_CTRL        KMOD_CTRL
#define KM_SHIFT       KMOD_SHIFT
#define KM_ALT         KMOD_ALT

typedef struct input_s
{
	SDLMod mod;
	SDLKey sym;
	Uint16 uni;
#if 0
	int modifier;
	int key;
	struct input_s *next; // maybe use this to make input chains like: press 
	                      // ctl+a then press q and it will do something press
	                      // ctl+a then press w and it will do something else
	                      // However, this may be inherent in the menu system
	                      // pbwafer currently uses.  If you pressed ctl+a it
	                      // would go to that menu then when you pressed q it
	                      // would do that menu entry.
#endif
} input_t;

extern int input_chain[6];
extern input_t g_chain;

void input_init();
int input_handle();
int input_check();

// Return 0 for no difference
int input_diff(input_t *i1, input_t *i2);

void input_display_state(SDL_KeyboardEvent *key);
void input_display_modifiers(SDL_KeyboardEvent *key);
void input_display_key(SDL_KeyboardEvent *key);
char *input_getkeyname(input_t *i);

#endif /* !__INPUT_H */
