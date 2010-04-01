#include "star.h"

#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "math.h"
#include "utils.h"

/*************************************************************
 * Generates a star for the obj struct
 *************************************************************/
struct star_t *gen_star (char class, struct posSys_t *loc) {
	struct star_t *s;
	s = (struct star_t *)malloc(sizeof(*s));

	s->chData = loc;
	s->class = class;

	int i, j, r, shell;
	float shell_rat = 0.75;
	
	if (class == A_CLASS)
		r = 20;
	if (class == B_CLASS)
		r = 30;
	if (class == C_CLASS)
		r = 40;
	
	float shell_r = r * shell_rat;
	
	loc->lSize = 2 * r;
	loc->wSize = loc->lSize; 
	malloc_map(loc->lSize, loc->wSize, &(loc->map));
	
	double newR = 0;
	float iO, jO, mySqrt;
	double myatan, mySin, myCos;
	int seed = loc->xPos * loc->yPos;
	
	
	for (i = 0; i < r * 2; i++) {
		if ((i >= r - shell_r) && (i <= r * 2 - (r - shell_r)))
			shell = sqrt((shell_r * shell_r) - ((i-r) * (i-r))) * ratio;
		else
			shell = 0;
		
		for (j = 0; j < r * ratio * 2; j++) {
			//Sets iO and jO to the distance i and j are from 0
			iO = abs(i-r);
			jO = abs(j-r);
			//returns angle between the current point and the origin
			myatan = atan(iO/jO);
			mySin = get_rand(seed, 1, r/6)*sin(myatan);
			seed = get_rand(seed, 0, 500);
			myCos = get_rand(seed, 1, r/6)*cos(myatan);
			seed = get_rand(seed, 0, 500);
			//calculates the new polar radius
			newR = r - fabs(mySin) - fabs(myCos);
			
			mySqrt = sqrt((iO*iO)+(jO*jO));
			if (mySqrt < r) {
				if ((mySqrt > shell_r) && (mySqrt < newR))
					loc->map[i][j] = '*';
				else if (mySqrt <= shell_r)
					loc->map[i][j] = '@';
				else loc->map[i][j] = ' ';
			}
			else
				loc->map[i][j] = ' ';
		}
	}

	//TODO: Does this line do anything?
	s->chData = loc;
	return s;
}
