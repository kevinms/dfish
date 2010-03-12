/*************************************************************
 * obj_map.c
 * 
 * Implements stuff for global and local objects
 * 
 *************************************************************/

#include "obj_map.h"


int init_objMap (FILE *objFile, union map_obj *mapObjs) {
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
			if (strcmp(temp, comp) == 0)
				type = OBJ_STAR;
			comp = "OBJ_PLANET";
			if (strcmp(temp, comp) == 0)
				type = OBJ_PLANET;
			comp = "OBJ_ASTEROID";
			if (strcmp(temp, comp) == 0)
				type = OBJ_ASTEROID;
			comp = "OBJ_NEBULA";
			if (strcmp(temp, comp) == 0)
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

			mapObjs = init_obj(type, class, newSys);
			mapObjs++;
			numObjs++;
			assert((newSys = malloc(sizeof(*newSys))) != NULL);
		}
	}
	
	
	
	
	return 1;
}

union map_obj *init_obj (int type, char class, struct posSys_t *loc) {
	union map_obj *myObj;
	assert((myObj = malloc (sizeof(*myObj))) != NULL);
	
	FILE *charData;
	assert((charData = fopen("dataz/obj_planet.dat", "r")) != 0);
	
	if (type == OBJ_PLANET) {
		//designate as landsc_t in union
		//fill in type and class
		//read chardata
	}
	
	fclose(charData);
	
	return (myObj);
}


//add function that adds objs to map//

