/***************************************************
 * keys.h
 * 
 * Inits functions for handling keyboard events
 * 
 ***************************************************/

#include "map.h"


void upKeyEvent (SDL_Event *event, struct posSys_t *GPS, struct posSys_t *LPS);

void downKeyEvent (SDL_Event *event, struct posSys_t *GPS, struct posSys_t *LPS);
