#include "menu.h"

#include <stdlib.h>
#include "utils.h"
#include "render.h"

int id = 0;

menu_t *MENU_init(const char *name)
{
	menu_t *m;
	m = (menu_t *)malloc(sizeof(*m));

	m->id = id;
	m->par = NULL;
	m->name = p_strcpy(name);
	m->kc.key = 'c';
	m->entry_list = list_init();
	m->callback = NULL;

	//MENU_add_entry(m,"Cancel",{NONE,' '},NULL);
	//MENU_add_entry(m,"Down",{CTRL,DOWN},NULL);
	//MENU_add_entry(m,"Up",{CTRL,UP},NULL);

	id++;

	return m;
}

menu_t *MENU_add_entry(menu_t *par,const char *name,keychain_t kc,void (*callback)())
{
	menu_t *m;
	m = (menu_t *)malloc(sizeof(*m));

	m->id = id;
	m->par = par;
	m->name = p_strcpy(name);
	m->kc = kc;
	m->entry_list = list_init();
	m->callback = callback;

	list_add(par->entry_list,(void *)m);

	id++;

	return m;
}

void MENU_load(menu_t *m)
{
	link_t *tmp;

	R_move(0,0);

	R_stringln(m->name);
	R_stringln("");

	for(tmp = m->entry_list->head; tmp != NULL; tmp = tmp->next)
		R_stringln(((menu_t *)(tmp->item))->name);
}

void MENU_free(menu_t *m)
{

}
