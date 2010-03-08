/***************************************************
 * keys.c
 * 
 * Implements functions for handling keyboard events
 * 
 ***************************************************/

#include "keys.h"

/***************************************************
 * 
 * Handles Key Releasing Events, mainly shift keys
 * 
 ***************************************************/
void upKeyEvent 	(SDL_Event *event, struct posSys_t *GPS, struct posSys_t *LPS) {
	
	
}


/***************************************************
 * 
 * Handles Key Pressing Events
 * 
 ***************************************************/
void downKeyEvent	(SDL_Event *event, struct posSys_t *GPS, struct posSys_t *LPS) {
	SDL_keysym keyPressed = event->key.keysym;
	if ((keyPressed.sym == KEY_UP) 
			|| (keyPressed.sym == KEY_DOWN) 
			|| (keyPressed.sym == KEY_LEFT) 
			|| (keyPressed.sym == KEY_RIGHT)) {
			LPS = move_map(keyPressed.sym, GPS, LPS, 1);
			LPS = fix_map(GPS, LPS);
			display_map(GPS, LPS);
	}
	else if (keyPressed.sym == KEY_TAB) {
		exit(1);
	}
	display_map(GPS, LPS);
}



void shiftKeyEvent	(SDL_Event *event, struct posSys_t *GPS, struct posSys_t *LPS) {
	
	
	
}
