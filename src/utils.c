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
	x = (rand_r(&y) % (max - min) + min);
	return x;
}
