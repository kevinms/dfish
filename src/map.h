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


void malloc_map(int length, int width, int ***map);
struct posSys_t *init_map (int lSize, int wSize, char *arg);
struct posSys_t *fix_map (struct posSys_t *GPS, struct posSys_t *LPS);
struct posSys_t *move_map (int keyPressed, struct posSys_t *GPS, struct posSys_t *LPS, int num);
void display_map (struct posSys_t *GPS, struct posSys_t *LPS);
void write_map (char *fName, struct posSys_t *GPS);