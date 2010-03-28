/*************************************************************
 * map.h
 * 
 * Declares structs and stuff for the global and local map
 * 
 *************************************************************/
#ifndef MAP_H_
#define MAP_H_

/* Defines defaults for LPS and GPS structures */
#define lLDefault 41
#define wLDefault 61
#define lGDefault 200
#define wGDefault 250

/* Positioning System struct */
struct posSys_t {
	int lSize;
	int wSize;
	int **map;
	int xPos;
	int yPos;
};

void malloc_map(int length, int width, int ***map);
struct posSys_t *init_map (int lSize, int wSize, char *arg);
struct posSys_t *fix_map (struct posSys_t *GPS, struct posSys_t *LPS);
struct posSys_t *move_map (int keyPressed, struct posSys_t *GPS, struct posSys_t *LPS, int num);
void display_map (struct posSys_t *GPS, struct posSys_t *LPS);
void write_map (char *fName, struct posSys_t *GPS);

#endif /* !MAP_H_ */
