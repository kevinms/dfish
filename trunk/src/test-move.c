/*
 * Tests moving around within a local system
 * and a global system of ascii char's 
 * 
 */

#include "map.h"

int main (int argc, char *argv[]) {
	struct posSys_t *GPS, *LPS;
	GPS = init_map(500, 500, 1);
	LPS = init_map(lLDefault, wLDefault, 0);
	
	LPS = move_map(GPS, LPS);
	
	return 0;
}
