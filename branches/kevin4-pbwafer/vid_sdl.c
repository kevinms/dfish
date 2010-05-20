#include "vid.h"

#include "SDL.h"

void VID_init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		fprintf(stderr,"SDL Video subsystem failed to initialize: %s\n",SDL_GetError());
}

void VID_init_mode(vidmode_t *mode)
{
	SDL_Surface *screen;

	// The real width and height of the window is provided here.  This is the 
	// root screen.  All other screen's will be fast blitted onto this one.
	if((screen = SDL_SetVideoMode(mode->width,mode->height,mode->bpp,SDL_SWSURFACE)) == NULL)
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

int initscrext(int w,int h,bool real)
{
	//TODO: i need some way of keeping track of what the width and height of the
	// screen is.  Since we are implementing multiple views this kind of info
	// should be handled per view...
	TC_W=TC_Screen->w/TC_XSpace;
	TC_H=TC_Screen->h/TC_YSpace;
	TC_LayersShown[0]=true;

	for(x=1;x<7;x++)
		TC_LayersShown[x]=false;

	return 0;
};
