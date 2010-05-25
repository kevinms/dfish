#ifndef __MENU_H
#define __MENU_H

#include "list.h"

//TODO: get the input system working for at the very least the keyboard events
//TODO: get keychains up and running, make sure it is robust enough
//TODO: get some sort of command structure running

typedef struct keychain_s
{
	int modifier;
	int key;
	// possibly a modifier
	// some type of series of keys
} keychain_t;

typedef struct menu_s
{
	int id;
	struct menu_s *par;
	const char *name;
	keychain_t kc;
	list_t *entry_list;
	void (*callback)(); //TODO: change this to be some sort of command,
	                    //      there could be a giant amount of commands like
	                    //      load this item at this location or fire this
	                    //      weopon here, fire this weapon and track the
	                    //      target, load menu command, 
} menu_t;

// New menus should have a few default entries (cancel, back, main menu)
menu_t *MENU_init(const char *name);
menu_t *MENU_add_entry(menu_t *par,const char *name,keychain_t kc,void (*callback)());
void MENU_load(menu_t *m);
void MENU_free(menu_t *m);

#endif /* !__MENU_H */
/*
// This way is slightly more condensed and it will be able to be more robust
// There will be a need for global keychains, 
menu_t *menu;
menu = MENU_new("Main Menu");
menu = MENU_add_entry(menu,"Clear View",{KEYCHAIN_HERE},CMD_submenu);
	menu = MENU_add_entry(menu->par,"Main View",{KEYCHAIN_HERE},CMD_clearview(v1));
	menu = MENU_add_entry(menu->par,"Chat View",{KEYCHAIN_HERE},CMD_clearview(v2));
menu = MENU_add_entry(menu->par->par,"Generate Galaxy",{KEYCHAIN_HERE},CMD_gen_galaxy());
*/
