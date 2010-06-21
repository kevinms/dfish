#include "vid.h"

void VID_init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		fprintf(stderr,"SDL Video subsystem failed to initialize: %s\n",SDL_GetError());
}

void VID_init_mode(vidmode_t *mode)
{
	// The real width and height of the window is provided here.  This is the 
	// root screen.  All other screen's will be fast blitted onto this one.
	if((mode->screen = SDL_SetVideoMode(mode->width,mode->height,mode->bpp,SDL_SWSURFACE)) == NULL)
		fprintf(stderr,"Video initialization failed: %s\n",SDL_GetError());
}

void VID_set_caption(char *name)
{
	SDL_WM_SetCaption(name, NULL);
}

void VID_shutdown()
{
	SDL_Quit();
}
