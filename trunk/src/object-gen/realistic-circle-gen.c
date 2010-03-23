/*************************************************************
 * realistic-circle-gen.c
 * 
 * Generates a realistic circle
 * 
 *************************************************************/


#include "../teahf.h"

int gen_circle (struct map_objS *obj) {
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
	loc->wSize = loc->lSize * ratio - 1;
	malloc_map(loc->lSize, loc->wSize, &(loc->map));
	
	for (i = 0; i < r * 2; i++) {
		k = sqrt((r * r) - ((i-r) * (i-r))) * ratio;
		
		for (j = 0; j < r * ratio * 2; j++) {
			if(j < 0 + r * ratio) {
				if(j < (r*ratio) - k) {
					loc->map[i][j] = ' ';
				}
				else {
					loc->map[i][j] = '@';
				}
			}
			else if(j >= r*ratio) {
				if (j < k + r*ratio) {
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
