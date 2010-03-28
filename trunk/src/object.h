#ifndef OBJECT_H_
#define OBJECT_H_

#include "TinyCurses.h"
#include "map.h"

struct attrib_t {
	int mass;
	//struct composition;
	int temperature;
	int health;
};

struct object_t {
	int type;
	void *data;
	struct posSys_t *chData;
};

struct object_t *object_init (int type, char class, struct posSys_t *loc);
int hit_check (struct posSys_t *chData, struct posSys_t *GPS);

#endif /* !OBJECT_H_ */
