#include "blueprint.h"

#include <stdlib.h>

#include "render.h"

bp_t *BP_init()
{
	bp_t *b;

	b = (bp_t *)malloc(sizeof(*b));
	b->mass = 0;

	b->hull = NULL;
	b->center[0] = NULL;
	b->center[1] = NULL;
	b->center[2] = NULL;

	b->head = NULL;

	return b;
}

entity_t *BP_add_entity(bp_t *b, int id)
{
	entity_t *e;

	if(b == NULL)
		return NULL;

	e = (entity_t *)malloc(sizeof(*e));
	e->id = id;

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
	R_mvsetch(e->ch,x+e->offset_x,y+e->offset_y);
}

void BP_render(bp_t *b)
{
	entity_t *tmp;

	for(tmp = b->head; tmp != NULL; tmp = tmp->next)
		BP_render_ent(tmp,tmp->offset_x,tmp->offset_y);
}
