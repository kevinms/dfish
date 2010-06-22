#include <stdio.h>

#include "pbwafer/pbwafer.h"
#include "pbwafer/vid.h"
#include "pbwafer/view.h"
#include "pbwafer/cmd.h"
#include "pbwafer/net.h"
#include "pbwafer/proto.h"
#include "pbwafer/console.h"

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
	PBWAFER_init(INIT_VID|INIT_RENDER|INIT_INPUT|INIT_NET|INIT_PROTO);
	vm = VID_init_mode(700,480,32,"dfish");

	// Initilize 3 views
	VSPACE_init(0,0,500,280,"./Font.ttf",7,vm->screen,7);
	VSPACE_menu_init(500,0,200,480,"./Font_default.ttf",10,vm->screen,7);
	VCONSOLE_init(0,280,500,200,"./Font_default.ttf",10,vm->screen,7);

	PROTO_set_clientinfo(STATE_NOP,"newplayer");

	// Input Loop
	for(;;) {
		if(PROTO_recv() > 0)
			PROTO_client_parse_DGRAM();

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
