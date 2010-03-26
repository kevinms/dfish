/*************************************************************
 * obj_map.c
 * 
 * Implements stuff for global and local objects
 * 
 *************************************************************/

#include "teahf.h"

/*************************************************************
 * Searches a list of names in an array and returns 
 * 		the index of the name matching the token
 *************************************************************/
int getndx (char *arr[], char *token) {
	int i = 0;
	int count = 0;
	while ((strcasecmp(token, arr[i]) != 0) && (count == 0)) {
			i++;
	}
	return (i);
}

/*************************************************************
 * Returns a random positive int, from the interval [x,max] 
 *************************************************************/
int get_rand (int x, int max) {
	
	return x;
}

/*************************************************************
 * Adds objects to map, and creates a map_objS
 *************************************************************/
struct map_objS *init_obj (int type, char class, struct posSys_t *loc) {
	struct map_objS *myObj;
	assert((myObj = malloc (sizeof(*myObj))) != NULL);
	assert((myObj->type = malloc(sizeof(*(myObj->type)))) != NULL);
	
	switch (type) {
		case OBJ_PLANET:
			assert((myObj->type->landscape = malloc(sizeof(*(myObj->type->landscape)))) != NULL);
			myObj->type->landscape->type = type;
			myObj->type->landscape->class = class;
			myObj->type->landscape->chData = loc;
			
			gen_planet(myObj);
			break;
		case OBJ_STAR:
			assert((myObj->type->landscape = malloc(sizeof(*(myObj->type->landscape)))) != NULL);
			myObj->type->landscape->type = type;
			myObj->type->landscape->class = class;
			myObj->type->landscape->chData = loc;
			
			gen_star(myObj);
			break;
	}

	return (myObj);
}


/*************************************************************
 * Initializes the system map using data gleaned from the
 * input file and posts it into the GPS
 *************************************************************/
int init_objMap (FILE *objFile, struct posSys_t *GPS, struct system_t *mySys) {
	mySys->numObjs = 0;
	
	int xPos, yPos;
	int height, width;
	int numObjs = 0;
	char class = A_CLASS;
	int type = OBJ_ASTEROID;
	struct posSys_t *newSys;
	assert((newSys = malloc(sizeof(*newSys))) != NULL);
	
	char temp[20];
	char temp2[20];
	char *comp = "MAP_DATA";
	char *escape = "/escape";
	
	//Beware, getndx will segfault if char * not recognized
	//                      0         1      2     3    4       5       6
	char *nameArr[] = {"NUM_OBJS", "TYPE", "XY", "HW", ";", "/escape", NULL};
	int ndx;

	assert(fscanf(objFile, "%s", temp) == 1);
	if (strcmp(comp, temp) != 0)
		return 0;

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
					mySys->sysObjs->archeType = ARCH_LANDSC;
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
				newSys->xPos = xPos;
				newSys->yPos = yPos;
				newSys->lSize = height;
				newSys->wSize = width;
				
				
				mySys->sysObjs = init_obj(type, class, newSys);
				mySys->sysObjs->ptrGPS = GPS;
				add_obj(mySys->sysObjs);
				mySys->sysObjs++;
				mySys->numObjs++;
				newSys++;
				assert((newSys = malloc(sizeof(*newSys))) != NULL);
				break;
		}
	}

	return 1;
}


/*************************************************************
 * Adds an object to the GPS
 *************************************************************/
void add_obj (struct map_objS *myObj) {
	int i,j;
	struct posSys_t *objLoc;

	switch (myObj->archeType) {
		case ARCH_LANDSC:
			objLoc = myObj->type->landscape->chData;
			break;
		case ARCH_UNIT:
			objLoc = myObj->type->unit->chData;
			break;
		case ARCH_ITEM:
			objLoc = myObj->type->item->chData;
			break;
		default:
			objLoc = myObj->type->landscape->chData;
	}

	for(i = 0; i < objLoc->lSize; i++) {
		for(j = 0; j < objLoc->wSize; j++) {
			if (objLoc->map[i][j] != ' ')
				myObj->ptrGPS->map[i + objLoc->yPos][j + objLoc->xPos] = objLoc->map[i][j];
		}
	}
}
