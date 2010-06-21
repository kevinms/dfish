#include <stdio.h>

#include "pbwafer/vid.h"
#include "pbwafer/view.h"
#include "pbwafer/render.h"
#include "pbwafer/menu.h"
#include "pbwafer/input.h"
#include "pbwafer/list.h"
#include "pbwafer/console.h"
#include "pbwafer/buf.h"
#include "pbwafer/cmd.h"
#include "pbwafer/net.h"
#include "pbwafer/proto.h"

net_t *netserv;

menu_t *menu,*tmenu;
view_t *v1, *v2, *v3;

int main(void)
{
	// setup global commands
	cmd_t g_cmds[1] = 
	{
		{{KM_NONE,K_BACKQUOTE},CONSOLE_toggle,NULL}
	};

	// initialize the video system
	vidmode_t vm;
	vm.width = 700;
	vm.height = 480;
	vm.bpp = 32;
	VID_init();
	VID_init_mode(&vm);
	VID_set_caption("pbwafer - test driver");

	atexit(SDL_Quit);

	// initilize systems
	R_init();
	input_init();
	NET_init();
	PROTO_init();

	vnew = VSPACE_init(0,0,500,280,"./Font.ttf",7,vm.screen,7);
	vnew = VSPACE_menu_init(500,0,200,480,"./Font_default.ttf",10,vm.screen,7);
	vnew = VCONSOLE_init(0,280,500,200,"./Font_default.ttf",10,vm.screen,7);

	menu_view = v3;
	VIEW_focus(v1);
	//active_view = v2;

	v1->accept_input = mainview_accept_input;

	PROTO_set_clientinfo(STATE_NOP,"pughar");

	// Input Loop
	cmd_t *c = NULL;
	int ret;

	for(;;) {
		if(PROTO_recv() > 0)
			PROTO_client_parse_DGRAM();

		ret = input_handle();
			if(ret == 2) {
				// Check the global commands
				c = CMD_find_key_array(g_cmds,&g_chain,1);
				if(c != NULL) {
					CMD_do(c);
				} else {
					active_view->accept_input();
				}
			} else if(ret == 0){
				return 0;
			}
	}

	return 0;
}
