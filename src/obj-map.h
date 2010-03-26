/*************************************************************
 * obj_map.h
 * 
 * Declares structs and externs for 
 * the global and local map objects
 * 
 *************************************************************/
#ifndef OBJ_MAP_H_
#define OBJ_MAP_H_

#include "TinyCurses.h"

/* Define Archetypes for the structure */
#define ARCH_LANDSC		100
#define ARCH_UNIT		200
#define ARCH_ITEM		300

/* Define  space stuff to be 0-19 */
#define OBJ_STAR		0
#define OBJ_PLANET		1
#define OBJ_ASTEROID		2
#define OBJ_NEBULA		3

/* Define different types of units to be 20-49 */
#define OBJ_UNIT		20
#define OBJ_ENEMY		21
#define OBJ_CREATURE		22

/* Define different types of items to be 50-99 */
#define OBJ_ITEM		50

/* Define object classes */
#define A_CLASS 'A'
#define B_CLASS 'B'
#define C_CLASS 'C'

/* Positioning System struct */
struct posSys_t {
	int lSize;
	int wSize;
	int **map;
	int xPos;
	int yPos;
};

/* Landscape Obj struct */
struct landsc_t {
	int type;
	struct posSys_t *chData;
	char class;
	//add more stuff
};

/* Stats/Status struct */
struct status_t {
	int type;
	char class;
	char *effect;
	///add more stuff
};

/* Items & Objs struct */
struct item_t {
	int type;
	struct posSys_t *chData;
	char class;
	char *name;
	struct status_t *status;
	///add more stuff
};

/* Player Unit struct */
struct unit_t {
	int type;
	struct posSys_t *chData;
	char class;
	struct item_t *inventory;
	struct status_t *status;
	///add more stuff
};

/* Map Object union */
union map_obj {
	struct unit_t *unit;
	struct landsc_t *landscape;
	struct item_t *item;
};

struct map_objS {
	int archeType;
	union map_obj *type;
	struct posSys_t *ptrGPS;
	
};

struct system_t {
	struct map_objS *sysObjs;
	int numObjs;
	struct posSys_t *sysData;
	struct posSys_t *ptrGPS;
};

int getndx (char *arr[], char *token);
int get_rand (int x, int min, int max);
struct map_objS *init_obj (int type, char class, struct posSys_t *loc);
void init_newMap(struct system_t *emptySys);
int init_objMap (FILE *objFile, struct posSys_t *GPS, struct system_t *mySys);
void add_obj (struct map_objS *myObj);

#endif /* !OBJ_MAP_H_ */
