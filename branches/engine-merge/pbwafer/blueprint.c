#include "blueprint.h"

#include <stdlib.h>

#include "render.h"

prim_t primitives[2] = {{'O',0,255,0,0,0,0},{'_',255,255,0,0,0,0}};

bp_t *BP_init()
{
	bp_t *b;

	b = (bp_t *)malloc(sizeof(*b));

	b->head = NULL;
	b->center[0] = NULL;
	b->center[1] = NULL;
	b->center[2] = NULL;
	b->hull = NULL;
	b->mass = 0;
	b->x = 0;
	b->y = 0;

	return b;
}

entity_t *BP_init_ent()
{
	entity_t *e;

	e = (entity_t *)malloc(sizeof(*e));

	e->id = 0;
	e->mass = 0;
	e->health = 0;
	e->p = NULL;
	e->n = NULL;
	e->s = NULL;
	e->e = NULL;
	e->w = NULL;
	e->offset_x = 0;
	e->offset_y = 0;
	e->next = NULL;

	return e;
}

entity_t *BP_add_entity(bp_t *b, int id, int x, int y)
{
	entity_t *e;

	if(!b)
		return NULL;

	e = BP_init_ent();
	e->id = id;
	e->p = &primitives[id];
	e->offset_x = x - b->x;
	e->offset_y = y - b->y;

	// Insert into the linked list
	if(b->head == NULL) {
		b->head = e;
		b->center[0] = e;
	}
	else {
		e->next = b->head;
		b->head = e;
	}

	return e;
}

void BP_render_ent(entity_t *e,int x, int y)
{
	prim_t *p = e->p;
	//R_mvsetch(e->ch,x+e->offset_x,y+e->offset_y);
	R_setchext(p->ch,x+e->offset_x,y+e->offset_y,p->r,p->g,p->b,p->fr,p->fg,p->fb);
}

void BP_render(bp_t *b)
{
	entity_t *tmp;

	for(tmp = b->head; tmp != NULL; tmp = tmp->next)
		BP_render_ent(tmp,b->x,b->y);
}
