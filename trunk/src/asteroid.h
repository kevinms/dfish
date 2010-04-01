#ifndef ASTEROID_H_
#define ASTEROID_H_

#include "map.h"

struct asteroid_t {
	struct posSys_t *chData;
	struct attrib_t *attribs;
	char class;
};

struct asteroid_t *gen_asteroid (char class, struct posSys_t *loc);

#endif /* !NEBULA_H_ */
