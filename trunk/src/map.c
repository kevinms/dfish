/*************************************************************
 * map.c
 * 
 * Sets up structs and stuff for the global and local map
 * 
 *************************************************************/

#include "teahf.h"

/*******************************************************************************
 *
 * mallocs space for a length * width map
 * 
 * NOTE: you must either pass a pointer to the map or the address of the map
 * 	eg:  malloc_map(length,width,&map);
 ******************************************************************************/

void malloc_map(int length, int width, int ***map)
{
	assert(((*map) = (int**)malloc (sizeof(**map)*length)) != NULL);
	int i;
	for (i = 0; i < length; i++)
		assert(((*map)[i] = malloc (sizeof(***map) * width)) != NULL);
	
}

/************************************************
 * Initializes the posSys that passes through
 * changes setup based on the int variable
 * 		from setting up GPS to LPS
 ************************************************/
struct posSys_t *init_map (int lSize, int wSize, char *arg) {
	char ch = ' ';
	int i, j;
	char *lps = "lps";
	struct posSys_t *pSys;
	assert((pSys = malloc(sizeof(*pSys))) != NULL);
	int **myMap;

	malloc_map(lSize,wSize,&myMap);

	if (arg == NULL) {
		for(i = 0; i < lSize; i++) {
			for(j = 0; j < wSize; j++) {
				myMap[i][j] = ch;
				if (!(i % 5)) /* every 5th line is all '-' */
					myMap[i][j] = '-';
				if (!(j % 5)) /* every 5th column is all '|' */
					myMap[i][j] = '|';
			}
		}
		pSys->lSize = lSize;
		pSys->wSize = wSize;
	}
	
	/* Sets up LPS if setup = 0 */
	else if (strcmp(arg, lps) == 0) {
		for(i = 0; i < lSize; i++)
		pSys->xPos = 0;
		pSys->yPos = 0;
		pSys->lSize = lSize;
		pSys->wSize = wSize;
	}
	
	pSys->map = myMap;
	return pSys;
}

/**********************************************
 * assigns the proper GPS values to LPS
 * according to LPS->(x and y Positions)
 **********************************************/
struct posSys_t *fix_map (struct posSys_t *GPS, struct posSys_t *LPS) {
	int i;
	for (i = 0; i < LPS->lSize; i++)
		LPS->map[i] = GPS->map[i + LPS->yPos] + LPS->xPos;

	return LPS;
}


/**********************************************
 * Changes LPS pointers according to 
 * 		the offset (x and y Pos)
 **********************************************/
struct posSys_t *move_map (int keyPressed, struct posSys_t *GPS, struct posSys_t *LPS, int num) {
	if (keyPressed == KEY_UP) {
		if (LPS->yPos >= num)
			LPS->yPos -= num;
		else
			LPS->yPos = 0;
	}

	if (keyPressed == KEY_DOWN) {
		if ((LPS->yPos + LPS->lSize + num) < GPS->lSize)
			LPS->yPos += num;
		else
			LPS->yPos = GPS->lSize - LPS->lSize;
	}

	if (keyPressed == KEY_RIGHT) {
		if ((LPS->xPos + LPS->wSize + num) < GPS->wSize)
			LPS->xPos += num;
		else
			LPS->xPos = GPS->wSize - LPS->wSize;
	}

	if (keyPressed == KEY_LEFT) {
		if (LPS->xPos >= num)
			LPS->xPos -= num;
		else
			LPS->xPos = 0;
	}

	return LPS;
}

/* Displays the map */
void display_map (struct posSys_t *GPS, struct posSys_t *LPS) {
	clear();
	int i, j;
	for(i = 0; i < LPS->lSize; i++) {
		for(j = 0; j < LPS->wSize; j++) {
			printw("%c", LPS->map[i][j]);
		}
	}
	refresh();
}

/* Writes the map data out to a File */
void write_map (char *fName, struct posSys_t *GPS) {
	FILE *outFile;
	assert((outFile = fopen (fName, "w")) != NULL);

	fprintf(outFile, "%d ", GPS->lSize);
	fprintf(outFile, "%d ", GPS->wSize);
	fprintf(outFile, "\n");

	int i, j;
	for(i = 0; i < GPS->lSize; i++) {
		for(j = 0; j < GPS->wSize; j++) {
			fwrite ((GPS->map[i]+j), 1, sizeof(char), outFile);
		}
		fprintf(outFile, "\n");
	}
	fclose(outFile);
}
