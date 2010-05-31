#ifndef __MENU_H
#define __MENU_H

#include "list.h"

//TODO: get the input system working for at the very least the keyboard events
//TODO: get keychains up and running, make sure it is robust enough
//TODO: get some sort of command structure running

#include "input.h"
#include "cmd.h"
#include "view.h"

typedef struct menu_s
{
	int id;
	struct menu_s *par;
	const char *name;
	list_t *entry_list;
	struct menu_s *cur;

	struct menu_s *cancel;

	cmd_t c;
	struct menu_s *root;
} menu_t;

extern view_t *menu_view;

menu_t *MENU_init(const char *name);
menu_t *MENU_add_menu(menu_t *par,menu_t *root,const char *name,int mod, int sym);
menu_t *MENU_init_entry(menu_t *par,menu_t *root,const char *name,int mod,int sym,void (*callback)(),void *data);
menu_t *MENU_add_entry(menu_t *par,menu_t *root,const char *name,int mod,int sym,void (*callback)(),void *data);
menu_t *MENU_do_key(menu_t *root, input_t *in);
void MENU_load(menu_t *m);
void MENU_free(menu_t *m);

void MENU_do_load_menu(void *data);
void MENU_do_load_par(void *data);

#endif /* !__MENU_H */
