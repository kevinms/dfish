/*************************************************************
 * realistic-circle-gen.c
 * 
 * Generates a realistic circle
 * 
 *************************************************************/


#include "../teahf.h"


/*************************************************************
 * Generates a planet for the obj struct
 *************************************************************/
int gen_planet (struct map_objS *obj) {
	FILE *write;
   write = fopen("out.txt", "w");
	
	struct posSys_t *loc;
	loc = obj->type->landscape->chData;
	char class = obj->type->landscape->class;
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
	obj->type->landscape->chData = loc;
	return 0;
}

/*************************************************************
 * Generates a star for the obj struct
 *************************************************************/
int gen_star (struct map_objS *obj) {
	FILE *write;
   write = fopen("out.txt", "w");
	
	struct posSys_t *loc;
	loc = obj->type->landscape->chData;
	char class = obj->type->landscape->class;
	int i, j, k, r, shell;
	float shell_rat = 0.75;
	
	if (class == A_CLASS)
		r = 6;
	if (class == B_CLASS)
		r = 12;
	if (class == C_CLASS)
		r = 18;
	
	float shell_r = r * shell_rat;
	
	loc->lSize = 2 * r;
	loc->wSize = loc->lSize /* ratio - 1*/; 
	malloc_map(loc->lSize, loc->wSize, &(loc->map));
	
	
	float fi, fj, fr;
	
	
	for (i = 0; i < r * 2; i++) {
		k = sqrt((r * r) - ((i-r) * (i-r))) * ratio;
		if ((i >= r - shell_r) && (i <= r * 2 - (r - shell_r)))
			shell = sqrt((shell_r * shell_r) - ((i-r) * (i-r))) * ratio;
		else
			shell = 0;
		
		for (j = 0; j < r * ratio * 2; j++) {
			
			fi = i;
			fj = j;
			fr = r;
			double myatan = atan((fi-fr)/(fj-fr));
			
			if(j < r * ratio) {
				if(i == r && j == 0)
					loc->map[i][j] = ' ';
				else if(j < (r*ratio) - k) {
					loc->map[i][j] = ' ';
				}
				else {
					if (j > r * ratio - shell -1 )
						loc->map[i][j] = '@';
					else {
						if (myatan > (PI / 10))
							if (myatan > (PI / 2.75))
								loc->map[i][j] = '|';
							else
								loc->map[i][j] = '\\';
						else if (fabs(myatan) > (PI / 10))
							if (fabs(myatan) > (PI / 2.75))
								loc->map[i][j] = '|';
							else
								loc->map[i][j] = '/';
						else
							loc->map[i][j] = '~';
loc->map[i][j] = '*';
					}
				}
			}
			else if(j >= r*ratio) {
				if (i == r && j == r * 2 - 1)
					loc->map[i][j] = ' ';
				else if (j < k + r*ratio) {
					if (j < (r * ratio + shell))
						loc->map[i][j] = '@';
					else {
						if (myatan > (PI / 10))
							if (myatan > (PI / 2.75))
								loc->map[i][j] = '|';
							else
								loc->map[i][j] = '\\';
						else if (fabs(myatan) > (PI / 10))
							if (fabs(myatan) > (PI / 2.75))
								loc->map[i][j] = '|';
							else
								loc->map[i][j] = '/';
						else
							loc->map[i][j] = '~';
loc->map[i][j] = '*';
					}
				}
				else {
					loc->map[i][j] = ' ';
				}
			}
		}
	}
	obj->type->landscape->chData = loc;
	return 0;
}

/*************************************************************
 * Checks to see if the two obj structs intersect in any way
 *************************************************************/
//implement checking for non-space vacuum chars later on
int hit_check (struct posSys_t *chData, struct posSys_t *GPS) {
	int i,j;
	
	for (i = 0; i < chData->lSize; i++) {
		for (j = 0; j < chData->wSize; j++) {
			if ((GPS->map[i+chData->yPos][j+chData->xPos] != ' ')
				&& (chData->map[i][j] != ' '))
				return -1;
		}
	}
	return 0;
}
