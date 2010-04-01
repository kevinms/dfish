#include "utils.h"

#include "SDL.h"

/*************************************************************
 * Searches a list of names in an array and returns 
 * 		the index of the name matching the token
 *************************************************************/
int getndx (char *arr[], char *token) {
	int i = 0;
	int count = 0;
	while ((strcasecmp(token, arr[i]) != 0) && (count == 0)) {
			i++;
	}
	return (i);
}

/*************************************************************
 * Returns a random positive int, from the interval [x,max] 
 *************************************************************/
int get_rand (int x, int min, int max) {
	Uint32 y = x;
	int z;
	z = (rand_r(&y) % (max - min) + min);
	return z;
}

/*************************************************************
 * Returns a random positive or negative 1
 *************************************************************/
int get_negpos (int x) {
	Uint32 y = x;
	int z;
	z = (rand_r(&y) % (2 + 1) - 1);
	if (z != 0)
		return z;
	return (get_negpos(x+1));
}
