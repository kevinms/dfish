/*************************************************************
 * map.c
 * 
 * Sets up structs and stuff for the global and local map
 * 
 *************************************************************/

#include "map.h"
#include <assert.h>


/************************************************
 * Initializes the posSys that passes through
 * changes setup based on the int variable
 * 		from setting up GPS to LPS
 ************************************************/
struct posSys_t *init_map (int lSize, int wSize, int setup) {
	char ch = 'a';
	int k = 1;
	int i, j;
	
	struct posSys_t *pSys;
	assert((pSys = malloc(sizeof(*pSys))) != NULL);
	int **myMap;
	assert((myMap = malloc(sizeof(*myMap)*lSize)) != NULL);
	
	if (setup == 1) {
		for(i = 0; i < lSize; i++) {
			assert((*(myMap+i) = malloc(sizeof(**myMap)*wSize)) != NULL);
			for(j = 0; j < wSize; j++) {
				myMap[i][j] = ch;
				if (k == 1)
					ch++;
				else
					ch--;
					
				if (ch == 'z' && k == 1)
					k = -1;
				if (ch == 'a' && k == -1)
					k = 1;
/************	every 4th line is all '#'		**************/
				if (!(i % 6))
					myMap[i][j] = '#';
/************	every 4th column is all '|'		**************/
				if (!(j % 4))
					myMap[i][j] = '|';
			}
		}
	}
	
/************	Sets up LPS if setup = 0		**************/
	else if (setup == 0) {
		for(i = 0; i < lSize; i++)
			assert((*(myMap+i) = malloc(sizeof(**myMap)*wSize)) != NULL);
		pSys->xPos = 0;
		pSys->yPos = 0;
	}
	pSys->lSize = lSize;
	pSys->wSize = wSize;
	pSys->map = myMap;
	return pSys;
}

/**********************************************
 * assigns the proper GPS values to LPS
 * according to LPS->(x and y Positions)
 **********************************************/
struct posSys_t *fix_map (struct posSys_t *GPS, struct posSys_t *LPS) {
	int i;
	for (i = 0; i < LPS->lSize; i++) {
		LPS->map[i] = GPS->map[i + LPS->yPos] + LPS->xPos;
	}
	
	return LPS;
}


/**********************************************
 * Changes LPS pointers according to 
 * 		the offset (x and y Pos)
 **********************************************/
struct posSys_t *move_map (int keyPressed, struct posSys_t *GPS, struct posSys_t *LPS, int num) {
	if (keyPressed == KEY_UP) {
		if (LPS->yPos >= num)
			LPS->yPos -= num;
		else
			LPS->yPos = 0;
	}
		
	if (keyPressed == KEY_DOWN) {
		if ((LPS->yPos + LPS->lSize + num) < GPS->lSize)
			LPS->yPos += num;
		else
			LPS->yPos = GPS->lSize - LPS->lSize - num;
	}
		
	if (keyPressed == KEY_RIGHT) {
		if ((LPS->xPos + LPS->wSize + num) < GPS->wSize)
			LPS->xPos += num;
		else
			LPS->xPos = GPS->wSize - LPS->wSize - num;
	}
		
	if (keyPressed == KEY_LEFT) {
		if (LPS->xPos >= num)
			LPS->xPos -= num;
		else
			LPS->xPos = 0;
	}
	
	return LPS;
}


/**************		Displays the map 	**************/
void display_map (struct posSys_t *GPS, struct posSys_t *LPS) {
	clear();
	int i, j;
	for(i = 0; i < LPS->lSize; i++) {
		for(j = 0; j < LPS->wSize; j++) {
			printw("%c", LPS->map[i][j]);
		}
	}
	refresh();	
}


/******		Writes the map data out to a File	 ****/
void write_map (char *fName, struct posSys_t *GPS) {
	FILE *outFile;
	assert((outFile = fopen (fName, "w")) != NULL);
	
	fprintf(outFile, "%d ", GPS->lSize);
	fprintf(outFile, "%d ", GPS->wSize);
	
	int i, j;
	for(i = 0; i < GPS->lSize; i++) {
		for(j = 0; j < GPS->wSize; j++) {
			fwrite ((GPS->map[i]+j), 1, sizeof(char), outFile);
		}
	}
	fclose(outFile);
}
