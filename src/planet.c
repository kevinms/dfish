#include "planet.h"

#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "constants.h"

/*************************************************************
 * Generates a planet for the obj struct
 *************************************************************/
struct planet_t *gen_planet (char class, struct posSys_t *loc) {
	struct planet_t *p;
	p = (struct planet_t *)malloc(sizeof(*p));

	p->chData = loc;
	p->class = class;

	int i, j, r;
	
	if (class == A_CLASS)
		r = 6;
	if (class == B_CLASS)
		r = 12;
	if (class == C_CLASS)
		r = 18;
	
	loc->lSize = 2 * r;
	loc->wSize = loc->lSize /* ratio - 1*/; 
	malloc_map(loc->lSize, loc->wSize, &(loc->map));
	
	float iO, jO;
	
	for (i = 0; i < r * 2; i++) {
		
		for (j = 0; j < r * ratio * 2; j++) {
			//Sets iO and jO to the distance i and j are from 0
			iO = abs(i-r);
			jO = abs(j-r);
			
			if ((sqrt((iO*iO)+(jO*jO)) < r) || (iO == 0 && jO == 0))
				loc->map[i][j] = '@';
			else
				loc->map[i][j] = ' ';
		}
	}

	//TODO: Does this line do anything?
	p->chData = loc;
	return p;
}
