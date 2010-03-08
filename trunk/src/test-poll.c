/*
 * Tests event gets with while loops
 * 
 * 
 */

#include "keys.h"
#include "SDL.h"

int main (int argc, char *argv[]) {
	struct posSys_t *GPS, *LPS;
	GPS = init_map(lGDefault, wGDefault, 1);
	LPS = init_map(lLDefault, wLDefault, 0);
	
	LPS = fix_map(GPS, LPS);
	int i,j;
	initscrext(wLDefault, lLDefault, false);
	refresh();
	for(i = 0; i < LPS->lSize; i++) {
		for(j = 0; j < LPS->wSize; j++){
			printw("%c", LPS->map[i][j]);
		}
	}
	
	move(0,0);
	refresh();

	int keyPressed = 0;
	SDL_Event test_event;
	
	while (keyPressed != KEY_TAB) {
		
/************************************************************
 * 
 * Put in normal operating functions like AI paths and stuff
 * 
*************************************************************/
		
		if (SDL_PollEvent(&test_event)) {
			
			switch (test_event.type) {
				case SDL_KEYDOWN:
					downKeyEvent(&test_event, GPS, LPS);
					break;
				default: 
					break;
			}
		}
		
	}

/*	
	int keyPressed = 0;
	while (keyPressed != KEY_TAB) {
		keyPressed = getch();
		if ((keyPressed == KEY_UP) 
			|| (keyPressed == KEY_DOWN) 
			|| (keyPressed == KEY_LEFT) 
			|| (keyPressed == KEY_RIGHT)) {
			LPS = move_map(keyPressed, GPS, LPS, 1);
			LPS = fix_map(GPS, LPS);
			
			clear();
			for(i = 0; i < LPS->lSize; i++) {
				for(j = 0; j < LPS->wSize; j++) {
					printw("%c", LPS->map[i][j]);
				}
			}
			move(0,0);
			refresh();
		}
	}
*/
	return 0;
}
