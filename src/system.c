#include "system.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "utils.h"

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

	int i, count = 0, objNdx = 0;
	for (i = 0; i < emptySys->numObjs; i++) {
		xPos = get_rand(objNdx, 0, emptySys->ptrGPS->wSize - 50);
		yPos = get_rand(objNdx, 0, emptySys->ptrGPS->lSize - 50);
		if (i == 0)
			type = OBJ_STAR;
		else 
			type = get_rand(objNdx, 1, 2); //TODO: add asteroids and nebulae
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
printf("%d %d %d %d %c\n", emptySys->numObjs, type, xPos, yPos, class);
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
					//mySys->sysObjs[objNdx].archeType = ARCH_LANDSC;
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
