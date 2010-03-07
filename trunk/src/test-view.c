#include "TinyCurses.h"

#define MSIZE 100
int main (int argc, char *argv[])
{
	char ch = 'a';
	int i, j;
	char map[MSIZE][MSIZE];
	for(i = 0; i < MSIZE; i++)
		for(j = 0; j < MSIZE; j++)
			map[i][j] = 'a';
	
	//printw("Curses!\n");
	for(; ch <= 'z'; ch++) {
		move(0,0);
		
		for(i = 0; i < MSIZE; i++)
			for(j = 0; j < MSIZE; j++)
				printw("%c",ch);
		refresh();
		//getch();
	}
	
	return 0;
};
