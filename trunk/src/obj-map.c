/*************************************************************
 * obj_map.c
 * 
 * Implements stuff for global and local objects
 * 
 *************************************************************/

#include "teahf.h"


/*************************************************************
 * Adds objects to map, and creates a map_objS
 *************************************************************/
struct map_objS *init_obj (int type, char class, struct posSys_t *loc) {
	struct map_objS *myObj;
	assert((myObj = malloc (sizeof(*myObj))) != NULL);
	assert((myObj->type = malloc(sizeof(*(myObj->type)))) != NULL);
	
	FILE *charData;
	assert((charData = fopen("dataz/obj_planet.dat", "r")) != 0);
	if (type == OBJ_PLANET) {
		assert((myObj->type->landscape = malloc(sizeof(*(myObj->type->landscape)))) != NULL);
		myObj->type->landscape->type = type;
		myObj->type->landscape->class = class;
		
		malloc_map(loc->lSize, loc->wSize, &(loc->map));
		
		char temp;
		int i,j;
		for(i = 0; i < loc->lSize; i++) {
			for(j = 0; j < loc->wSize; j++) {
				assert((fscanf(charData, "%c", &temp)) == 1);
				if (temp == '\n')
					assert((fscanf(charData, "%c", &temp)) == 1);
				loc->map[i][j] = temp;
			}
		}
		myObj->type->landscape->chData = loc;
	}
	
	fclose(charData);
	
	return (myObj);
}



int init_objMap (FILE *objFile, struct map_objS *mapObjs, struct posSys_t *GPS) {
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
	
	
	assert(fscanf(objFile, "%s", temp) == 1);
	if (strcmp(comp, temp) != 0)
		return 0;
	
	while (strcmp(escape, temp) != 0) {
		assert(fscanf(objFile, "%s", temp) == 1);
		
		comp = "NUM_OBJS";
		if (strcmp(temp, comp) == 0)
			assert(fscanf(objFile, "%d", &numObjs) == 1);
		
		comp = "TYPE";
		if (strcmp(temp, comp) == 0) {
			assert(fscanf(objFile, "%s", temp2) == 1);
			comp = "OBJ_STAR";
			if (strcmp(temp2, comp) == 0)
				type = OBJ_STAR;
			comp = "OBJ_PLANET";
			if (strcmp(temp2, comp) == 0) {
				mapObjs->archeType = ARCH_LANDSC;
				type = OBJ_PLANET;
			}
			comp = "OBJ_ASTEROID";
			if (strcmp(temp2, comp) == 0)
				type = OBJ_ASTEROID;
			comp = "OBJ_NEBULA";
			if (strcmp(temp2, comp) == 0)
				type = OBJ_NEBULA;
		}
		
		comp = "XY";
		if (strcmp(temp, comp) == 0)
			assert(fscanf(objFile, "%d %d", &xPos, &yPos) == 2);
		comp = "HW";
		if (strcmp(temp, comp) == 0)
			assert(fscanf(objFile, "%d %d", &height, &width) == 2);
		comp = ";";
		if (strcmp(temp, comp) == 0) {
			newSys->xPos = xPos;
			newSys->yPos = yPos;
			newSys->lSize = height;
			newSys->wSize = width;
			mapObjs->ptrGPS = GPS;
			
			mapObjs = init_obj(type, class, newSys);
			add_obj(mapObjs, GPS);
			mapObjs++;
			numObjs++;
			newSys++;
			assert((newSys = malloc(sizeof(*newSys))) != NULL);
		}
	}
	
	
	
	
	return 1;
}

void add_obj (struct map_objS *myObj, struct posSys_t *GPS) {
	int i,j;
	struct posSys_t *objLoc;
//	if (myObj->landscape->chData != NULL)
		objLoc = myObj->type->landscape->chData;
//	if (myObj->item->chData != NULL)
	//	objLoc = myObj->item->chData;
	//if (myObj->unit->chData != NULL)
	//	objLoc = myObj->unit->chData;
	
	for(i = 0; i < objLoc->lSize; i++) {
		for(j = 0; j < objLoc->wSize; j++) {
			if (objLoc->map[i][j] != ' ')
				GPS->map[i + objLoc->yPos][j + objLoc->xPos] = objLoc->map[i][j];
		}
		
	}
	
}
