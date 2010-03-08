/***************************************************
 * keys.c
 * 
 * Implements functions for handling keyboard events
 * 
 ***************************************************/

#include "keys.h"


/******    Defines defaults for Map Shifting Events	******/
#define numDefault 1
#define shiftDefault 5


/***************************************************
 * 
 * Handles Key Releasing Events
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
	int myNum = 1;
	
/****		Detects presence of Shift keys		****/
	if ((keyPressed.mod & KMOD_RSHIFT)
		|| (keyPressed.mod & KMOD_LSHIFT))
			myNum = 5;

	LPS = move_map(keyPressed.sym, GPS, LPS, myNum);
	LPS = fix_map(GPS, LPS);
	display_map(GPS, LPS);
	
//////NOTE: Exits only after tab and 1 other key is pressed//////
	if (keyPressed.sym == KEY_TAB) {
		exit(1);
	}
}
