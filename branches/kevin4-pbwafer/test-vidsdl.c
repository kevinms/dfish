#include <stdio.h>
#include "vid.h"

int main(void)
{
	struct vidmode_t vm;
	vm.width = 640;
	vm.height = 480;
	vm.bpp = 32;
	VID_Init();
	VID_InitMode(&vm);
	VID_SetCaption("pbwafer");

	for(;;);

	return 0;
}
