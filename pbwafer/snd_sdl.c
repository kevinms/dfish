#include "snd.h"
#include "console.h"

#include <SDL.h>
#include <SDL_mixer.h>

Mix_Music *music=NULL;

void SND_init(void)
{
	int buffer=1024;

	if(SDL_Init(SDL_INIT_AUDIO)<0) {
		fprintf(stderr,"SND_init failed: %s\n",SDL_GetError());
		return;
	}

	if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,buffer)<0)
		printf("Mix_OpenAudio: %s\n", Mix_GetError());

	Mix_VolumeMusic(SDL_MIX_MAXVOLUME);
}

void SND_shutdown(void)
{
	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

void SND_play_music(const char *f)
{
	if(music)
		SND_stop();

	if(!(music=Mix_LoadMUS(f))) {
		CONSOLE_print("Mix_LoadMUS: %s\n", Mix_GetError());
		return;
	}

	if(Mix_FadeInMusic(music, -1, 2000)==-1)
		printf("Mix_FadeInMusic: %s\n", Mix_GetError());
}

void SND_stop()
{
	Mix_HaltMusic();
	Mix_FreeMusic(music);
	music=NULL;
}

void SND_volume(int v)
{
	Mix_VolumeMusic(v);
}
