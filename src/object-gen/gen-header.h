/*************************************************************
 * gen-header.h
 * 
 * Declares stuff for generation files
 * 
 *************************************************************/

#include "../TinyCurses.h"

#define ratio 1
#define PI 3.141592653589793
#define BAD_LOC -1


/*************************************************************
 * dist * radius = minimum distance between objects
 * //later add dependencies on gravitational limitations//
 *************************************************************/
#define STAR_MINSAFEDIST 2
#define PLANET_MINSAFEDIST 1
#define AST_MINSAFEDIST 0.5

int gen_planet (struct map_objS *obj);
int gen_star (struct map_objS *obj);
int hit_check (struct posSys_t *chData, struct posSys_t *GPS);
