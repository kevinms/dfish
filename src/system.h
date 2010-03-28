#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "TinyCurses.h"
#include "list.h"
#include "object.h"

/* Stats/Status struct */
/*
struct status_t {
	int type;
	char class;
	char *effect;
	///add more stuff
};
*/

struct system_t {
	struct list_t *objects;
	int numObjs;
	struct posSys_t *ptrGPS;
};

struct system_t *system_init_procedural(struct posSys_t *GPS);
struct system_t *system_init_file(FILE *objFile, struct posSys_t *GPS);
void system_add_obj (struct object_t *myObj, struct posSys_t *GPS);

#endif /* !SYSTEM_H_ */
