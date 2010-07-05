#ifndef __VIEW_SPACE_H
#define __VIEW_SPACE_H

#include "pbwafer/menu.h"
#include "pbwafer/view.h"

typedef struct vspace_s {
	view_t *v;
	view_t *v_menu;
	menu_t *menu;
} vspace_t;

extern vspace_t *vspace;

void VSPACE_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);
void VSPACE_menu_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);
void VSPACE_update();
void VSPACE_accept_input();

// Space view callbacks
void CB_req_servinfo_bcast();
void CB_req_servinfo();
void CB_req_connect();
void CB_console_fuzz();
void CB_gengalaxy();

#endif /* !__VIEW_SPACE_H */
