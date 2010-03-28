/***************************************************
 * keys.c
 * 
 * Implements functions for handling keyboard events
 * 
 ***************************************************/
#include "keys.h"

/***************************************************
 * 
 * Handles Key Releasing Events
 * 
 ***************************************************/
void upKeyEvent(SDL_Event *event, struct posSys_t *GPS, struct posSys_t *LPS) {
	
	
}

/***************************************************
 * 
 * Handles Key Pressing Events
 * 
 ***************************************************/
void downKeyEvent(SDL_Event *event, struct posSys_t *GPS, struct posSys_t *LPS) {

	SDL_keysym keyPressed = event->key.keysym;
	int myNum = 5;

	/* Detects presence of Shift keys */
	if ((keyPressed.mod & KMOD_RSHIFT)
		|| (keyPressed.mod & KMOD_LSHIFT))
			myNum = 10;

	LPS = move_map(keyPressed.sym, GPS, LPS, myNum);
	LPS = fix_map(GPS, LPS);
	display_map(GPS, LPS);

	/* Exits with message "Reality Was Aborted" when tab or esc is pressed */
	if (keyPressed.sym == KEY_TAB || keyPressed.sym == KEY_ESCAPE) {
		printf("Reality Was ");
		abort();

	}
	if (keyPressed.sym == SDLK_w) {
		char *fName = "map.txt";
		write_map (fName, GPS);
	}
}
