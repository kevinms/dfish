/***************************************************
 * keys.h
 * 
 * Inits functions for handling keyboard events
 * 
 ***************************************************/

#include "TinyCurses.h"


/******    Defines defaults for Map Shifting Events	******/
#define numDefault 1
#define shiftDefault 5


void upKeyEvent (SDL_Event *event, struct posSys_t *GPS, struct posSys_t *LPS);

void downKeyEvent (SDL_Event *event, struct posSys_t *GPS, struct posSys_t *LPS);