#ifndef NEBULA_H_
#define NEBULA_H_

#include "map.h"

struct nebula_t {
	struct posSys_t *chData;
	struct attrib_t *attribs;
	char class;
};

struct nebula_t *gen_nebula (char class, struct posSys_t *loc);

#endif /* !NEBULA_H_ */
