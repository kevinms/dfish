#include "object.h"

#include <assert.h>
#include <stdlib.h>
#include "constants.h"
#include "planet.h"
#include "star.h"
#include "nebula.h"
#include "asteroid.h"

/*************************************************************
 * Adds objects to map, and creates a map_objS
 *************************************************************/
struct object_t *object_init(int type, char class, struct posSys_t *loc) {
	struct object_t *obj;
	assert((obj = malloc (sizeof(*obj))) != NULL);
	
	switch (type) {
		case OBJ_PLANET:
			obj->type = type;
			obj->data = gen_planet(class, loc);
			obj->chData = ((struct planet_t *)obj->data)->chData;
			break;
		case OBJ_STAR:
			obj->type = type;
			obj->data = gen_star(class, loc);
			obj->chData = ((struct star_t *)obj->data)->chData;
			break;
		case OBJ_NEBULA:
			obj->type = type;
			obj->data = gen_nebula(class, loc);
			obj->chData = ((struct nebula_t *)obj->data)->chData;
			break;
		case OBJ_ASTEROID:
			obj->type = type;
			obj->data = gen_asteroid(class, loc);
			obj->chData = ((struct asteroid_t *)obj->data)->chData;
			break;
	}

	return (obj);
}

/*************************************************************
 * Checks to see if the two obj structs intersect in any way
 *************************************************************/
//TODO: implement checking for non-space vacuum chars later on
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
