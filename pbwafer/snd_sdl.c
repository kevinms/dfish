// Copyright (C) 2011  Kevin Smith
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

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
