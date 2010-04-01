#include "system.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "math.h"
#include "constants.h"
#include "utils.h"
#include "map.h"
#include "star.h"

/*************************************************************
 * Initializes a new system map procedurally
 *************************************************************/
struct system_t *system_init_procedural(struct posSys_t *GPS) {
	struct system_t *emptySys;
	int min = 5;
	int max = 10;
	int class, type, xPos, yPos;
	struct posSys_t *newSys;
	struct object_t *currObj;
	int temp;

	assert((emptySys = malloc(sizeof(*emptySys))) != NULL);
	emptySys->numObjs = get_rand(0, min, max);
	emptySys->ptrGPS = GPS;
	emptySys->objects = list_init();
	
	//uses lowest system dimensions for radius
	int temp2 = emptySys->ptrGPS->lSize;
	if (emptySys->ptrGPS->wSize < emptySys->ptrGPS->lSize)
		temp2 = emptySys->ptrGPS->wSize;
	int radius = temp2 * ASTRO_UNIT;
	int newR = 0;

	int i, count = 0, objNdx = 0;
	for (i = 0; i < emptySys->numObjs; i++) {
		newR = radius * i;
		if (i == 0) {
			type = OBJ_STAR;
			xPos = get_rand(objNdx, emptySys->ptrGPS->wSize * RATIO_STARBOX, emptySys->ptrGPS->wSize * (RATIO_STARBOX + 0.5));
			yPos = get_rand(objNdx, emptySys->ptrGPS->lSize * RATIO_STARBOX, emptySys->ptrGPS->lSize * (RATIO_STARBOX + 0.5));
		}
		else {
			//TODO: maybe make the star ptr more accessible??
			struct object_t *ptr;
			struct star_t *ptr2;
			struct posSys_t *mapptr;
			ptr = (struct object_t *)emptySys->objects->head->item;
			ptr2 = (struct star_t *)ptr->data;
			mapptr = ptr2->chData;
			type = get_rand(objNdx + 5, 1, 4); //TODO: add asteroids
			
			
			int sentinel = 0;
			int mySeed = objNdx;
			while (sentinel == 0) {
				xPos = get_rand(mySeed + 1337, 0, newR);
				if (mapptr->xPos - xPos < 0)
					xPos = mapptr->xPos + get_negpos(mySeed*8) * xPos;
				else if (xPos + mapptr->xPos > GPS->wSize - 50)
					xPos = mapptr->xPos - xPos;
				else
					xPos = mapptr->xPos + get_negpos(mySeed * 1337) * xPos;
				int mySqrt = sqrt(newR*newR - (xPos-mapptr->xPos)*(xPos-mapptr->xPos));
				if (0 > mapptr->yPos - mySqrt)
					yPos = mapptr->yPos + mySqrt;
				else if (GPS->lSize - 50 < mapptr->yPos + mySqrt)
					yPos = mapptr->yPos - mySqrt;
				else
					yPos = mapptr->yPos + get_negpos(xPos*5)*mySqrt;
				
				//double checks val's
				if ((xPos < 0) || (xPos > GPS->wSize - 50)
					|| (0 > mapptr->yPos - mySqrt)
					|| (GPS->lSize - 50 < mapptr->yPos + mySqrt))
					sentinel = 0;
				else {
					sentinel = 1;
					printf("%d %d %d %d\n", emptySys->numObjs, type, xPos, yPos);
				}
				mySeed++;
			}
		}
		temp = get_rand(objNdx + 3, 0, 3);
		switch (temp) {
			case 0:
				class = A_CLASS;
				break;
			case 1:
				class = B_CLASS;
				break;
			case 2:
				class = C_CLASS;
				break;
			default:
				class = C_CLASS;
		}
		//kills obj generation if the radius has gone outside the box
		if (yPos > emptySys->ptrGPS->lSize - 50) {
			printf("fail\n");
			return emptySys;
		}
		assert((newSys = malloc(sizeof(*newSys))) != NULL);
		newSys->xPos = xPos;
		newSys->yPos = yPos;
		
		currObj = object_init(type, class, newSys);
		list_add(emptySys->objects,currObj);
		system_add_obj(currObj, emptySys->ptrGPS);
		objNdx++;
		newSys++; //TODO: not needed?
		count++;
	}
	return emptySys;
}

/*************************************************************
 * Initializes the system map using data gleaned from the
 * input file and posts it into the GPS
 *************************************************************/
struct system_t *system_init_file (char *filename, struct posSys_t *GPS) {
	int objNdx = 0;
	int xPos, yPos;
	int height, width;
	int numObjs = 0;
	char class = B_CLASS;
	int type = OBJ_ASTEROID;

	struct posSys_t *newSys;

	char temp[20];
	char temp2[20];
	char *comp = "MAP_DATA";
	char *escape = "/escape";
	
	struct object_t *currObj;

	FILE *objFile;
	assert((objFile = fopen(filename, "r")) != NULL);

	struct system_t *mySys;
	assert((mySys = malloc(sizeof(*mySys))) != NULL);
	mySys->numObjs = 0;
	mySys->ptrGPS = GPS;
	mySys->objects = list_init();

	//Beware, getndx will segfault if char * not recognized
	//                      0         1      2     3    4       5       6
	char *nameArr[] = {"NUM_OBJS", "TYPE", "XY", "HW", ";", "/escape", NULL};
	int ndx;

	assert(fscanf(objFile, "%s", temp) == 1);

	while (strcmp(escape, temp) != 0) {
		assert(fscanf(objFile, "%s", temp) == 1);

		ndx = getndx(nameArr, temp);
		switch (ndx) {

			case 0: /* NUM_OBJS */
				assert(fscanf(objFile, "%d", &numObjs) == 1);
				break;
			case 1: /* Type */
				assert(fscanf(objFile, "%s", temp2) == 1);
				comp = "OBJ_STAR";
				if (strcmp(temp2, comp) == 0)
					type = OBJ_STAR;
				comp = "OBJ_PLANET";
				if (strcmp(temp2, comp) == 0) {
					type = OBJ_PLANET;
				}
				comp = "OBJ_ASTEROID";
				if (strcmp(temp2, comp) == 0)
					type = OBJ_ASTEROID;
				comp = "OBJ_NEBULA";
				if (strcmp(temp2, comp) == 0)
					type = OBJ_NEBULA;
				break;
			case 2: /* XY */
				assert(fscanf(objFile, "%d %d", &xPos, &yPos) == 2);
				break;
			case 3: /* HW */
				assert(fscanf(objFile, "%d %d", &height, &width) == 2);
				break;
			case 4: /* ; */
				assert((newSys = malloc(sizeof(*newSys))) != NULL);

				newSys->xPos = xPos;
				newSys->yPos = yPos;
				newSys->lSize = height;
				newSys->wSize = width;
				
				currObj = object_init(type, class, newSys);
				list_add(mySys->objects,currObj);
				system_add_obj(currObj, mySys->ptrGPS);
				mySys->numObjs++;
				objNdx++;
				newSys++;
				break;
		}
	}
	fclose(objFile);

	return mySys;
}

/*************************************************************
 * Adds an object to the GPS
 *************************************************************/
void system_add_obj (struct object_t *myObj, struct posSys_t *GPS) {
	int i,j;
	struct posSys_t *objLoc;

	objLoc = myObj->chData;

	int seed = 0;
	while (hit_check(objLoc, GPS) == -1) {
		objLoc->yPos = get_rand(seed, 0, GPS->lSize - objLoc->lSize);
		objLoc->xPos = get_rand(seed, 0, GPS->wSize - objLoc->wSize);
		seed++;
	}
	printf("%d\n", hit_check(objLoc, GPS));
	for(i = 0; i < objLoc->lSize; i++) {
		for(j = 0; j < objLoc->wSize; j++) {
			if (objLoc->map[i][j] != ' ')
				GPS->map[i + objLoc->yPos][j + objLoc->xPos] = objLoc->map[i][j];
		}
	}
}
