#include "planet.h"

#include "math.h"
#include "constants.h"

/*************************************************************
 * Generates a planet for the obj struct
 *************************************************************/
struct planet_t *gen_planet (char class, struct posSys_t *loc) {
	FILE *write;
	write = fopen("out.txt", "w");

	struct planet_t *p;
	p = (struct planet_t *)malloc(sizeof(*p));

	p->chData = loc;
	p->class = class;

	int i, j, k, r;
	
	if (class == A_CLASS)
		r = 6;
	if (class == B_CLASS)
		r = 12;
	if (class == C_CLASS)
		r = 18;
	
	loc->lSize = 2 * r;
	loc->wSize = loc->lSize /* ratio - 1*/; 
	malloc_map(loc->lSize, loc->wSize, &(loc->map));
	
	for (i = 0; i < r * 2; i++) {
		k = sqrt((r * r) - ((i-r) * (i-r))) * ratio;
		
		for (j = 0; j < r * ratio * 2; j++) {
			if(j < 0 + r * ratio) {
				if(i == r && j == 0)
					loc->map[i][j] = ' ';
				else if(j < (r*ratio) - k) {
					loc->map[i][j] = ' ';
				}
				else {
					loc->map[i][j] = '@';
				}
			}
			else if(j >= r*ratio) {
				if (i == r && j == r * 2 - 1)
					loc->map[i][j] = ' ';
				else if (j < k + r*ratio) {
				
					loc->map[i][j] = '@';
				}
				else {
					loc->map[i][j] = ' ';
				}
			}
		}
	}

	//TODO: Does this line do anything?
	p->chData = loc;
	return p;
}
