/*************************************************************
 * map.h
 * 
 * Declares structs and stuff for the global and local map
 * 
 *************************************************************/

#include "TinyCurses.h"

/******    Defines defaults for LPS and GPS structures	******/
#define lLDefault 40
#define wLDefault 60
#define lGDefault 200
#define wGDefault 250


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


void malloc_map(int length, int width, int ***map);
struct posSys_t *init_map (int lSize, int wSize, char *arg);
struct posSys_t *fix_map (struct posSys_t *GPS, struct posSys_t *LPS);
struct posSys_t *move_map (int keyPressed, struct posSys_t *GPS, struct posSys_t *LPS, int num);
void display_map (struct posSys_t *GPS, struct posSys_t *LPS);
void write_map (char *fName, struct posSys_t *GPS);
