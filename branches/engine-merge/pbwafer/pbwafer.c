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
