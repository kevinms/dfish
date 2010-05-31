#include "input.h"

#include <stdio.h>

int input_chain[6];
input_t g_chain;

void
input_init()
{
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);
}

int
input_handle()
{
	SDL_Event event;
	int running = 1;

	while(SDL_PollEvent(&event)) {
		switch(event.type){
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				g_chain.sym = event.key.keysym.sym;
				g_chain.mod = event.key.keysym.mod;
				return 2;
			case SDL_QUIT:
				running = 0;
				break;
		}
	}

	return running;
}

#if 0
int input_check()
{
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		switch(event.type){
			case SDL_KEYDOWN:
				input_display_state(&event.key);
				input_display_modifiers(&event.key);
				input_display_key(&event.key);
				break;
			case SDL_KEYUP:
				break;
			case SDL_QUIT:
				break;
		}
	}

	//return event;
}
#endif

int input_diff(input_t *in1, input_t *in2)
{
	if(in1->sym == in2->sym) {
		if(in1->mod == in2->mod) {
			return 0;
		}
	}
	return 1;
}

void
input_display_state(SDL_KeyboardEvent *key)
{
	if (key->type == SDL_KEYUP)
		printf("RELEASED: ");
	else
		printf("PRESSED: ");
}

void
input_display_modifiers(SDL_KeyboardEvent *key)
{
	SDLMod modifier = key->keysym.mod;
	if( modifier & KMOD_NUM ) printf( "NUMLOCK " );
	if( modifier & KMOD_CAPS ) printf( "CAPSLOCK " );
	if( modifier & KMOD_MODE ) printf( "MODE " );
	if( modifier & KMOD_LCTRL ) printf( "LCTRL " );
	if( modifier & KMOD_RCTRL ) printf( "RCTRL " );
	if( modifier & KMOD_LSHIFT ) printf( "LSHIFT " );
	if( modifier & KMOD_RSHIFT ) printf( "RSHIFT " );
	if( modifier & KMOD_LALT ) printf( "LALT " );
	if( modifier & KMOD_RALT ) printf( "RALT " );
	if( modifier & KMOD_LMETA ) printf( "LMETA " );
	if( modifier & KMOD_RMETA ) printf( "RMETA " );
}

void
input_display_key(SDL_KeyboardEvent *key)
{
	printf( "%s\n", SDL_GetKeyName(key->keysym.sym));
}

char *input_getkeyname(input_t *i)
{
	return SDL_GetKeyName(i->sym);
}

