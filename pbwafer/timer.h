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

#ifndef __TIMER_H
#define __TIMER_H

#include <SDL.h>

typedef struct pwtimer_s {
	Uint32 interval;
	Uint32 end_time;
} pwtimer_t;

void TIMER_init(pwtimer_t *t,Uint32 interval);
char TIMER_is_timeout(pwtimer_t *t);

#endif /* !__TIMER_H */
