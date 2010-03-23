/********************************************************
 * test-poll.c
 * 
 * Tests event gets with while loops
 * 
 ********************************************************/

#include "teahf.h"

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
	struct map_objS *mapObjs;
	int numObjs;
	assert((mapObjs = malloc(sizeof(*mapObjs))) != NULL);
	
	assert((objFile = fopen("dataz/obj_data.dat", "r")) != NULL);
	assert((numObjs = init_objMap(objFile, mapObjs, GPS)) != 0);

	fclose(objFile);
		
	LPS = fix_map(GPS, LPS);
	int i,j;
	initscrext(wLDefault, lLDefault, false);
	SDL_WM_SetCaption( "Dfish", NULL );
	refresh();
	for(i = 0; i < LPS->lSize; i++) {
		for(j = 0; j < LPS->wSize; j++){
			printw("%c", LPS->map[i][j]);
		}
	}
printf("width:%d\nheight:%d\n",TC_XSpace,TC_YSpace);
	move(0,0);
	refresh();

	int keyPressed = -1;
	SDL_Event test_event;
	while(1){

		fprintf(stderr,"event %d = %d\n",test_event.key.keysym.sym,KEY_TAB);
		
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
		keyPressed = test_event.key.keysym.sym;
	}

	fprintf(stderr,"hi\n");
	exit(0);
	return 0;
	printf("asdfasfd\n");
}
