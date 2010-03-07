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
				**(myMap) = ch;
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
		pSys->lSize = lSize;
		pSys->wSize = wSize;
	}
	else if (setup == 0) {
		for(i = 0; i < lSize; i++)
			assert((*(myMap+i) = malloc(sizeof(**myMap)*wSize)) != NULL);
		pSys->lSize = 0;
		pSys->wSize = 0;
	}
	pSys->map = myMap;
	return pSys;
}

struct posSys_t *move_map (struct posSys_t *GPS, struct posSys_t *LPS) {
	int i, j;
	for (i = 0; i < LPS->lSize; i++)
		for (j = 0; j < LPS->wSize; j++) {
			LPS->map[i][j] = GPS->map[i + LPS->lSize][j + LPS->wSize];
		}
	
	return LPS;
}

