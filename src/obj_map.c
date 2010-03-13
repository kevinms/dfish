/*************************************************************
 * obj_map.c
 * 
 * Implements stuff for global and local objects
 * 
 *************************************************************/

#include "teahf.h"



union map_obj *init_obj (int type, char class, struct posSys_t *loc) {
	union map_obj *myObj;
	assert((myObj = malloc (sizeof(*myObj))) != NULL);
	
	FILE *charData;
	assert((charData = fopen("dataz/obj_planet.dat", "r")) != 0);
	if (type == OBJ_PLANET) {
		assert((myObj->landscape = malloc(sizeof(struct landsc_t))) != NULL);
		myObj->landscape->type = type;
		myObj->landscape->class = class;
		
		malloc_map(loc->lSize, loc->wSize, &(loc->map));
		
		char temp;
		int i,j;
		printf("%d %d\n %d %d\n", loc->lSize, loc->wSize, loc->xPos, loc->yPos);
		for(i = 0; i < loc->lSize; i++) {
			for(j = 0; j < loc->wSize; j++) {
				assert((fscanf(charData, "%c", &temp)) == 1);
				if (temp == '\n')
					assert((fscanf(charData, "%c", &temp)) == 1);
				printf("%c", temp);
				//if (temp != ' ')
					loc->map[i][j] = temp;
			}
		}
		myObj->landscape->chData = loc;
	}
	
	fclose(charData);
	
	return (myObj);
}



int init_objMap (FILE *objFile, union map_obj *mapObjs, struct posSys_t *GPS) {
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
			if (strcmp(temp2, comp) == 0)
				type = OBJ_PLANET;
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

			mapObjs = init_obj(type, class, newSys);
			add_obj(mapObjs, GPS);
			mapObjs++;
			numObjs++;
			assert((newSys = malloc(sizeof(*newSys))) != NULL);
		}
	}
	
	
	
	
	return 1;
}

void add_obj (union map_obj *myObj, struct posSys_t *GPS) {
	int i,j;
	struct posSys_t *objLoc;
//	if (myObj->landscape->chData != NULL)
		objLoc = myObj->landscape->chData;
//	if (myObj->item->chData != NULL)
	//	objLoc = myObj->item->chData;
	//if (myObj->unit->chData != NULL)
	//	objLoc = myObj->unit->chData;
	
	for(i = 0; i < objLoc->lSize; i++) {
			for(j = 0; j < objLoc->wSize; j++) {
				
				GPS->map[i + objLoc->yPos][j + objLoc->xPos] = objLoc->map[i][j];
			}
		}
	
}
