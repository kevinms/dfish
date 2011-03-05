#include "view_console.h"

#include <stdio.h>
#include <stdlib.h>

#include "pbwafer/console.h"
#include "pbwafer/proto.h"

#ifdef SOUND
#include "pbwafer/snd.h"
#endif // SOUND

view_t *v_console;

void VCONSOLE_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl)
{
	v_console = VIEW_init(x,y,real_w,real_h,fontname,fontsize,screen,numl);
	v_console->showcursor = 1;
	v_console->cursor = '_';

	// Initialize the console
	CONSOLE_init("PBwafer Console",PROTO_client_send_chat);
	CONSOLE_attach_to_view(v_console);

	// Register a bunch of commands
	CONSOLE_register_cmd("/name",1,PROTO_req_name,"string name (no spaces)","Change player name");
	CONSOLE_register_cmd("/connect",2,PROTO_connect_ip,"IP PORT","Connect to a server");
	CONSOLE_register_cmd("/disconnect",0,PROTO_disconnect,NULL,"Disonnect from a server");
	CONSOLE_register_cmd("/info",2,CB_req_servinfo_ip,"IP PORT","Request info from a server");
	CONSOLE_register_cmd("/info-bcast",1,CB_req_servinfo_broadcast,"PORT","Get info from all servers on LAN");
	CONSOLE_register_cmd("/netsim",1,CB_netsim,NULL,"Tells how to use the console");
#ifdef SOUND
	CONSOLE_register_cmd("/snd-volume",1,CB_snd_volume,"VOLUME (percent)","Set music volume");
	CONSOLE_register_cmd("/snd-stop",0,SND_stop,NULL,"Stop music");
	CONSOLE_register_cmd("/snd-play",1,CB_snd_play,"music filename (no spaces)","Play specific music file");
#endif // SOUND

	// Print a message for the user to see.
	CONSOLE_print("Type /help for the console guide.");
}

void CB_req_servinfo_ip(int count, const char **s)
{
	printf("'%s' '%s'\n",s[0],s[1]);
	PROTO_req_servinfo_ip(s[0], s[1]);
}

void CB_req_servinfo_broadcast(int count, const char **s)
{
	PROTO_req_servinfo_broadcast(s[0]);
}

void CB_netsim(int count, const char **s)
{
	PROTO_client_netsim(atoi(s[0]),SIM_DC|SIM_PL);
}

#ifdef SOUND
void CB_snd_volume(int count, const char **s)
{
	SND_volume(atoi(s[0]));
}

void CB_snd_play(int count, const char **s)
{
	SND_play_music(s[0]);
}
#endif // SOUND
