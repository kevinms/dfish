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

#include <stdio.h>

#include "pbwafer/pbwafer.h"
#include "pbwafer/vid.h"
#include "pbwafer/view.h"
#include "pbwafer/cmd.h"
#include "pbwafer/net.h"
#include "pbwafer/proto.h"
#include "pbwafer/console.h"

#ifdef SOUND
#include "pbwafer/snd.h"
#endif // SOUND

#include "view_space.h"
#include "view_console.h"

int main(void)
{
	vidmode_t *vm;
	cmd_t *c = NULL;
	int ret;

	// setup global commands
	cmd_t g_cmds[1] = 
	{
		{{KM_NONE,K_BACKQUOTE},CONSOLE_toggle,NULL}
	};

	// Initilize systems

#ifdef SOUND
	PBWAFER_init(INIT_VID|INIT_RENDER|INIT_INPUT|INIT_NET|INIT_PROTO|INIT_SND);
#else
	PBWAFER_init(INIT_VID|INIT_RENDER|INIT_INPUT|INIT_NET|INIT_PROTO);
#endif // SOUND

	vm = VID_init_mode(700,480,32,"dfish");

	// Initilize 3 views
	VSPACE_init(0,0,500,280,"./Font_default.ttf",10,vm->screen,7);
	VSPACE_menu_init(500,0,200,480,"./Font_default.ttf",10,vm->screen,7);
	VCONSOLE_init(0,280,500,200,"./Font_default.ttf",10,vm->screen,7);

#ifdef SOUND
	SND_play_music("sound/xonotic/rising-of-the-phoenix.ogg");
#endif // SOUND

	PROTO_set_clientinfo(STATE_NOP,"newplayer");

	// Input Loop
	for(;;) {
		if(PROTO_recv() > 0)
			PROTO_client_parse_DGRAM();

		PROTO_client_send_packets();

		ret = input_handle();
		if(ret == 2) {
			// Check the global commands
			c = CMD_find_key_array(g_cmds,&g_chain,1);
			if(c != NULL)
				CMD_do(c);
			else
				active_view->accept_input();
		} else if(ret == 0)
			return 0;
	}

	return 0;
}
