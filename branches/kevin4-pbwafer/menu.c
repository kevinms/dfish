#include "menu.h"

#include <stdlib.h>
#include "utils.h"
#include "render.h"

int id = 0;
view_t *menu_view;  // The most current view to render menu data to

menu_t *MENU_init(const char *name)
{
	menu_t *m;

	m = MENU_init_entry(NULL,NULL,name,KM_NONE,K_c,NULL,NULL);
	m->entry_list = list_init();

	m->cur = m;
	m->root = m;

	//MENU_add_entry(m,"Down",{CTRL,DOWN},NULL);
	//MENU_add_entry(m,"Up",{CTRL,UP},NULL);

	return m;
}

menu_t *MENU_add_menu(menu_t *par,menu_t *root,const char *name,int mod, int sym)
{
	menu_t *m;

	m = MENU_init_entry(par,root,name,mod,sym,MENU_load,NULL);
	m->c.data = (void *)m;
	m->entry_list = list_init();
	list_add(par->entry_list,(void *)m);

	m->cancel = MENU_init_entry(m,root,"Cancel",KM_NONE,K_SPACE,MENU_load,m->par);

	return m;
}

menu_t *MENU_init_entry(menu_t *par,menu_t *root,const char *name,int mod,int sym,void (*callback)(),void *data)
{
	menu_t *m;
	m = (menu_t *)malloc(sizeof(*m));

	m->id = id;
	m->par = par;
	m->name = p_strcpy(name);
	m->entry_list = NULL;

	m->c.i.mod = mod;
	m->c.i.sym = sym;
	printf("mod: %d, sys: %d\n",m->c.i.mod,m->c.i.sym);
	m->c.callback = callback;
	m->c.data = data;
	m->root = root;
	m->cancel = NULL;

	id++;

	return m;
}

menu_t *MENU_add_entry(menu_t *par,menu_t *root,const char *name,int mod,int sym,void (*callback)(),void *data)
{
	menu_t *m;

	m = MENU_init_entry(par,root,name,mod,sym,callback,data);
	list_add(par->entry_list,(void *)m);

	return m;
}

menu_t *MENU_do_key(menu_t *root, input_t *in)
{
	link_t *tmp;
	menu_t *m;
	m = root->cur;

	for(tmp = m->entry_list->head; tmp != NULL; tmp = tmp->next) {
		printf("yo: %d\n",((menu_t *)tmp->item)->c.i.sym);
		if(!input_diff(&(((menu_t *)tmp->item)->c.i),in)) {
			printf("menu_load\n");
			CMD_do(&((menu_t *)tmp->item)->c);
			return (menu_t *)tmp;
		}
	}

	if(m->cancel && !input_diff(&(m->cancel->c.i),in)) {
		printf("cmd_do\n");
		CMD_do(&m->cancel->c);
		return m->cancel;
	}
	return NULL;
}

void MENU_load(menu_t *m)
{
	link_t *tmp;

	m->root->cur = m;

	R_set(menu_view);
	R_clear();
	R_move(0,0);

	R_stringln(m->name);
	R_stringln("");

	for(tmp = m->entry_list->head; tmp != NULL; tmp = tmp->next) {
		R_string("(");
		R_string(input_getkeyname(&((menu_t *)(tmp->item))->c.i));
		R_string(") ");
		R_stringln(((menu_t *)(tmp->item))->name);
	}

	if(m->cancel) {
		R_stringln("");
		R_string("(");
		R_string(input_getkeyname(&(m->cancel->c.i)));
		R_string(") ");
		R_stringln(m->cancel->name);
	}

	R_update(); // May not want to call this here. Should this go in the
	            // main game logic loop only?
}

void MENU_free(menu_t *m)
{

}
