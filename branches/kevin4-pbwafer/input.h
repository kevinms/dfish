#ifndef __INPUT_H
#define __INPUT_H

// Goals for the input system:
//  - chainable key event system
//  - able to map keys to whatever commands the user wants
//  - some mouse support as well
//  - something similar to the circle selection in blender
//  - four to six keys can be pressed
//  - chainable key event system could use some

//TODO: Begin using SDL_GetKeyState() to find ALL keystates

#include "SDL.h"

#define KEY_UP        SDLK_UP
#define KEY_DOWN      SDLK_DOWN
#define KEY_LEFT      SDLK_LEFT
#define KEY_RIGHT     SDLK_RIGHT
#define KEY_HOME      SDLK_HOME
#define KEY_END       SDLK_END
#define KEY_PAGEUP    SDLK_PAGEUP
#define KEY_PAGEDOWN  SDLK_PAGEDOWN
#define KEY_BACKSPACE SDLK_BACKSPACE
#define KEY_ESCAPE    SDLK_ESCAPE
#define KEY_QUIT      SDL_QUIT
#define KEY_INSERT    SDLK_INSERT
#define KEY_SCROLL    SDLK_SCROLLOCK
#define KEY_PAUSE     SDLK_PAUSE
#define KEY_PRINT     SDLK_PRINT
#define KEY_TAB       SDLK_TAB
#define KEY_CAPSLOCK  SDLK_CAPSLOCK
#define KEY_DELETE    SDLK_DELETE
#define KEY_NUMLOCK   SDLK_NUMLOCK

#define KEY_CTRLDOWN  SDLK_RCTRL
#define KEY_CTRLUP    SDLK_LCTRL
#define KEY_ALTDOWN   SDLK_RALT
#define KEY_ALTUP     SDLK_LALT
#define KEY_SHIFTDOWN SDLK_RSHIFT
#define KEY_SHIFTUP   SDLK_LSHIFT

#define KEY_F1  SDLK_F1
#define KEY_F2  SDLK_F2
#define KEY_F3  SDLK_F3
#define KEY_F4  SDLK_F4
#define KEY_F5  SDLK_F5
#define KEY_F6  SDLK_F6
#define KEY_F7  SDLK_F7
#define KEY_F8  SDLK_F8
#define KEY_F9  SDLK_F9
#define KEY_F10 SDLK_F10
#define KEY_F11 SDLK_F11
#define KEY_F12 SDLK_F12

#define KEY_MOUSE1 1337
#define KEY_MOUSE2 1338
#define KEY_MOUSE3 1339
#define KEY_WHEELUP 1336
#define KEY_WHEELDOWN 1335

extern int input_chain[6];

void input_init();
int input_handle();
void input_check();

void input_display_state(SDL_KeyboardEvent *key);
void input_display_modifiers(SDL_KeyboardEvent *key);
void input_display_key(SDL_KeyboardEvent *key);

#endif /* !__INPUT_H */
