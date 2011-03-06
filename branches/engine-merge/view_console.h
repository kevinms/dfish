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

#ifndef __VIEW_CONSOLE_H
#define __VIEW_CONSOLE_H

#include "pbwafer/view.h"

extern view_t *v_console;

void VCONSOLE_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);

// Console command callbacks
void CB_req_servinfo_ip(int count, const char **s);
void CB_req_servinfo_broadcast(int count, const char **s);
void CB_netsim(int count, const char **s);

#ifdef SOUND
void CB_snd_volume(int count, const char **s);
void CB_snd_play(int count, const char **s);
#endif // SOUND

#endif /* !__VIEW_CONSOLE_H */
