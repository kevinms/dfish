/*************************************************************
 * obj_map.h
 * 
 * Declares structs and externs for 
 * the global and local map objects
 * 
 *************************************************************/

#include "TinyCurses.h"
#include <assert.h>

/***			Define  space stuff to be 0-19			***/
#define OBJ_STAR		0
#define OBJ_PLANET		1
#define OBJ_ASTEROID	2
#define OBJ_NEBULA		3

/***	Define different types of units to be 20-49		***/
#define OBJ_UNIT		20


/***	Define different types of items to be 50-99		***/
#define OBJ_ITEM		50

/***	Define object classes	***/
#define A_CLASS 'A'
#define B_CLASS 'B'


/*****		Positioning System struct		*****/
struct posSys_t {
	int lSize;
	int wSize;
	int **map;
	int xPos;
	int yPos;
};

/*********  	Landscape Obj struct	*********/
struct landsc_t {
	int type;
	struct posSys *chData;
	char class;
	///add more stuff
};

/*********  	Stats/Status struct		*********/
struct status_t {
	int type;
	char class;
	char *effect;
	///add more stuff
};

/*********  	Items & Objs struct		*********/
struct item_t {
	int type;
	struct posSys *chData;
	char class;
	char *name;
	struct status_t *status;
	///add more stuff
};

/*********  	Player Unit struct		*********/
struct unit_t {
	int type;
	struct posSys *chData;
	char class;
	struct item_t *inventory;
	struct status_t *status;
	///add more stuff
};

/*********		Map Object union		*********/
union map_obj {
	struct unit_t *unit;
	struct landsc_t *landscape;
	struct item_t *item;
};

int init_objMap (FILE *objFile, union map_obj *mapObjs);
union map_obj *init_obj (int type, char class, struct posSys_t *loc);
