/********************************************************
 * test-poll.c
 * 
 * Tests event gets with while loops
 * 
 ********************************************************/

#include "keys.h"
#include "SDL.h"

int main (int argc, char *argv[]) {
	
	char *lps = "lps";
	char *nil;
nil = 0; /* Archie avoiding a warning */
	
/********	Initializes local and global map	*********/
	struct posSys_t *GPS, *LPS;
	if (argc > 0)
		GPS = init_map(lGDefault, wGDefault, argv[1]);
	else
		GPS = init_map(lGDefault, wGDefault, nil);
	LPS = init_map(lLDefault, wLDefault, lps);
	
	
/*****		Initializing Map Objects		*************/
	FILE *objFile;
	assert((objFile = fopen("dataz/obj_data.dat", "r")) != NULL);
	assert(init_objMap(objFile) != 0);
	
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


	SDL_Event test_event;
	while(1){

		
/************************************************************
 * 
 * Put in normal operating functions like AI paths and stuff
 * This is the program daemon, it will run until we kill
 * 		it within the functions of the program
 * 
*************************************************************/
		
/******		Checks for events posted into the queue    ******/
		if (SDL_PollEvent(&test_event)) {
			switch (test_event.type) {
				case SDL_KEYDOWN:
					downKeyEvent(&test_event, GPS, LPS);
					break;

/* Exits with terminal message "Reality Was Aborted" when the (x) button is 
   clicked or the program is sent an alt + f4 signal			      */
				case SDL_QUIT:
					printf("Reality Was ");
					abort();
					break;
				default: 
					break;
			}
		}
	}

	return 0;
}
