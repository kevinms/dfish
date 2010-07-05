#ifndef __VIEW_FABRICATOR_H
#define __VIEW_FABRICATOR_H

#include "pbwafer/menu.h"
#include "pbwafer/view.h"
#include "pbwafer/blueprint.h"

typedef struct vfab_s {
	view_t *v;
	view_t *v_menu;
	menu_t *menu;

	int cursor_x;
	int cursor_y;

	bp_t *g_bp_edit;

	
} vfab_t;

extern vfab_t *vfab;

void VFAB_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);
void VFAB_menu_init(menu_t *r, int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);
void VFAB_update();
void VFAB_accept_input();

void CB_fabricator(void *data);
void CB_new_bp();
void CB_add_entity(int n);

#endif /* !__VIEW_FABRICATOR_H */
