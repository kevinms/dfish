/*
 * Declares structs and stuff for the global and local map
 * 
 * 
 * 
 *
 */

#include "TinyCurses.h"

#define lLDefault 20
#define wLDefault 30
#define lGDefault 200
#define wGDefault 250

//Positioning System struct
struct posSys_t {
	int lSize;
	int wSize;
	int **map;
	int xPos;
	int yPos;
};

struct posSys_t *init_map (int lSize, int wSize, int setup);
struct posSys_t *fix_map (struct posSys_t *GPS, struct posSys_t *LPS);
struct posSys_t *move_map (int keyPressed, struct posSys_t *GPS, struct posSys_t *LPS);
