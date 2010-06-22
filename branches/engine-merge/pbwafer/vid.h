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
vidmode_t *VID_init_mode(int w,int h,int bpp,const char *s);
void VID_set_caption(const char *name);
void VID_new_screen();

#endif /* !__VID_H */
