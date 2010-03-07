/*
 * Declares structs and stuff for the global and local map
 * 
 * 
 * 
 *
 */

#include "TinyCurses.h"

#define lLDefault 150
#define wLDefault 200
#define lGDefault 150
#define wGDefault 200

//Positioning System struct
struct posSys_t {
	int lSize;
	int wSize;
	int **map;
};

struct posSys_t *init_map (int lSize, int wSize, int setup);
int setup_map (struct posSys_t *GPS, struct posSys_t *LPS);
