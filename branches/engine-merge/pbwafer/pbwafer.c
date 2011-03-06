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

#include "pbwafer.h"

#include "net.h"
#include "proto.h"
#include "vid.h"
#include "render.h"
#include "input.h"

#ifdef SOUND
#include "snd.h"
#endif // SOUND

void PBWAFER_init(char mask)
{
	if(mask & INIT_NET)
		NET_init();
	if(mask & INIT_PROTO)
		PROTO_init();
	if(mask & INIT_VID)
		VID_init();
	if(mask & INIT_RENDER)
		R_init();
	if(mask & INIT_INPUT)
		input_init();
#ifdef SOUND
	if(mask & INIT_SND)
		SND_init();
#endif // SOUND
}

void PBWFAER_quit()
{
	VID_shutdown();
	printf("So long, and thanks for all the fish.\n");
	exit(1);
}
