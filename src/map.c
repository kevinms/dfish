/*
 * Sets up structs and stuff for the global and local map
 * 
 * 
 * 
 *
 */

#include "map.h"
#include <assert.h>

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
			}
		}
	}
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

struct posSys_t *fix_map (struct posSys_t *GPS, struct posSys_t *LPS) {
	int i, j;
	for (i = 0; i < LPS->lSize; i++)
		for (j = 0; j < LPS->wSize; j++) {
			LPS->map[i][j] = GPS->map[i + LPS->yPos][j + LPS->xPos];
		}
	
	return LPS;
}

struct posSys_t *move_map (int keyPressed, struct posSys_t *GPS, struct posSys_t *LPS) {
	if (keyPressed == KEY_UP)
		LPS->yPos += 3;
	if (keyPressed == KEY_DOWN)
		LPS->yPos -= 3;
	if (keyPressed == KEY_RIGHT)
		LPS->xPos += 3;
	if (keyPressed == KEY_LEFT)
		LPS->xPos -= 3;
	return LPS;
}
