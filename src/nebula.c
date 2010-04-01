#include "nebula.h"

#include <stdio.h>
#include <stdlib.h>
#include "math.h"
#include "constants.h"
#include "utils.h"

/*************************************************************
 * Generates a planet for the obj struct
 *************************************************************/
struct nebula_t *gen_nebula (char class, struct posSys_t *loc) {
	struct nebula_t *p;
	p = (struct nebula_t *)malloc(sizeof(*p));

	p->chData = loc;
	p->class = class;

	int i, j, r;
	
	if (class == A_CLASS)
		r = 10;
	if (class == B_CLASS)
		r = 18;
	if (class == C_CLASS)
		r = 26;
		
	double newR = 0;
	float iO = r;
	float jO = r;
	double myatan, mySin, myCos;
	int seed = loc->xPos * loc->yPos;
	
	
	loc->lSize = 2 * r;
	loc->wSize = loc->lSize; 
	malloc_map(loc->lSize, loc->wSize, &(loc->map));
	
	for (i = 0; i < r * 2; i++) {
		for (j = 0; j < r * ratio * 2; j++) {
			//Sets iO and jO to the distance i and j are from 0
			iO = abs(i-r);
			jO = abs(j-r);
			//returns angle between the current point and the origin
			myatan = atan(iO/jO);
			mySin = get_rand(seed, 1, 9)*sin(myatan);
			seed = get_rand(seed, 0, 500);
			myCos = get_rand(seed, 1, r/2)*cos(myatan);
			seed = get_rand(seed, 0, 500);
			//calculates the new polar radius
			newR = r - fabs(mySin) - fabs(myCos);
			
			if ((sqrt((iO*iO)+(jO*jO)) < newR) || (iO == 0 && jO == 0))
				loc->map[i][j] = '0';
			else
				loc->map[i][j] = ' ';
		}
	}

	//TODO: Does this line do anything?
	p->chData = loc;
	return p;
}
