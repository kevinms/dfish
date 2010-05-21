#ifndef __VID_H
#define __VID_H

#include "SDL.h"

typedef struct vidmode_s
{
	int width;
	int height;
	int bpp;
	SDL_Surface *screen;
} vidmode_t;

void VID_init(void);
void VID_shutdown(void);
void VID_init_mode(vidmode_t *mode);
void VID_set_caption(char *name);
void VID_new_screen();

#endif /* !__VID_H */
