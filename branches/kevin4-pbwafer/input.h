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

extern int input_chain[6];

void input_init();
int input_handle();

void input_display_state(SDL_KeyboardEvent *key);
void input_display_modifiers(SDL_KeyboardEvent *key);
void input_display_key(SDL_KeyboardEvent *key);

#endif /* !__INPUT_H */
