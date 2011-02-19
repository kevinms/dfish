#ifndef __BLUEPRINT_H
#define __BLUEPRINT_H
/*
All ship parts
All ship attributes (mass, #weapons, power, speed, #crew)
Damage to parts

Could send a diff of old a new blueprints if they ever change...
Objects are built from different entities or primitives

How many different types of entities will we have:
	2 bytes gives 65536 options

Data that should be stored in a blueprint:
	Our game with have up to 2^16 entities which could be referenced through a
	a 2 Byte short.  Those entities can be combined or grouped together to form
	other structure including but not limited to ships, bases, weapons, traps,
	etc... These entities should be stored somehow 

bblocks = entities

Entity position are stored relative to a base piece (e.g. +20x -20y)
The base piece can be either the visual center or center of mass (we can calculate this)
Objects that are intended to move must have all pieces connecting (we can give them an error telling them the requirements)
We can calculate the final velocity of the object and have it display in a stats window
For each entity we will have all four rotations of it stored (NSEW)

Blueprints will list entities pieces along with id (e.g. door = 42, id = 1), ID will help link entities
OR
Blueprints will list entities pieces (e.g. door = 42), entity position will help link entities
If an entity is moved or destoryed you search to destroy the link

// archie
We might need to check if a wall has been damanged enough for space to come in 

--------------------------------------------------------------------------------
Bulding Modes
--------------------------------------------------------------------------------
Weapon Blueprint Editor - This way weapons can have a seperate health bar
General Blueprint Editor - Can build either static xor moving objects


Building Blocks or bblock_t will be a single char piece, this can be anything
from a single armor piece to a wooden floor piece
*/

#define DELTA_ADD 0
#define DELTA_DMG 1
#define DELTA_DEL 2
#define DELTA_MOD 4
#define DELTA_POS 8

// There are basically 3 data structures in a blueprint...
//   - 2D Doubly-linked list
//   - Linked list
//   - Hash table

//TODO: Find if there is a good easy way to loop through a 2D Doubly-linked list

typedef struct prim_s {
	int ch;
	char r,g,b;
	char fr,fg,fb;
} prim_t;

typedef struct entity_s
{
	unsigned short id;
	unsigned int mass;
	unsigned int health;
	prim_t *p;

	// 2D Doubly-Linked List
	struct entity_s *n;
	struct entity_s *s;
	struct entity_s *e;
	struct entity_s *w;

	// Hashmap pieces
	int offset_x; // These could hashed and stored in a table
	int offset_y; // It would enable a much faster search

	// Linked List, will speed up looping through
	struct entity_s *next;
} entity_t;

typedef struct bp_s {
	entity_t *head;

	entity_t *center[3];

	entity_t *hull; // Just a pointer to a hull piece, this will hopefully speed
	                // up collision detection

	unsigned int mass;

	int x;
	int y;
} bp_t;

// Firing a weapon - [shot origin] [type] (speed)

typedef struct delta_s
{
	
} delta_t;

extern prim_t primitives[2];

bp_t *BP_init();
entity_t *BP_add_entity(bp_t *b, int id, int x, int y);

void BP_render_ent(entity_t *e,int x, int y);
void BP_render(bp_t *b);

delta_t *BP_delta(bp_t *bp1,bp_t *bp2);
void BP_apply_delta(bp_t *bp,delta_t *delta);

#endif /* !__BLUEPRINT_H */
