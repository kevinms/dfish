#ifndef STAR_H_
#define STAR_H_

#include "map.h"

struct star_t {
	struct posSys_t *chData;
	struct attrib_t *attribs;
	char class;
};

struct star_t *gen_star (char class, struct posSys_t *loc);

#endif /* !STAR_H_ */
