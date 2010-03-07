#include<stdlib.h>

/*******************************************************************************
* Name:		malloc_map
* Input:	2 ints (length and width)
*		***int (pointer to a map)
* Output:	void
* Purpose:	mallocs space for a length x width map
*******************************************************************************/
void malloc_map(int length, int width, int ***map)
{
	(*map) = (int**)malloc (sizeof(**map)*width);
	int i;
	for (i = 0; i < width; i++)
		map[i] = malloc (sizeof(***map) * length);
}
