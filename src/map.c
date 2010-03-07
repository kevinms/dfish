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
	}
	else if (setup == 0) {
		for(i = 0; i < lSize; i++)
			assert((*(myMap+i) = malloc(sizeof(**myMap)*wSize)) != NULL);
	}
	pSys->map = myMap;
	pSys->lSize = lSize;
	pSys->wSize = wSize;
	return pSys;
}

int setup_map (struct posSys_t *GPS, struct posSys_t *LPS) {
	
	
	return LPS;
}

