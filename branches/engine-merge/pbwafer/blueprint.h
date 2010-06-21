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

typedef struct bblock_s
{
	unsigned short id;
	int mass;
	int health;
	int link_id;
} bblock_t;

typedef struct blueprint_s
{
	int mass;
	int health;
	int center;

	list_t *bblock;
} blueprint_t;

#endif /* !__BLUEPRINT_H */
