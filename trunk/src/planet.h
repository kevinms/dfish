#ifndef PLANET_H_
#define PLANET_H_

#include "TinyCurses.h"
#include "map.h"

struct planet_t {
	struct posSys_t *chData;
	struct attrib_t *attribs;
	char class;
};

struct planet_t *gen_planet (char class, struct posSys_t *loc);

#endif /* !PLANET_H_ */
