#ifndef __RENDER_H
#define __RENDER_H

#include "view.h"

//TODO: Add all the functionality from TinyCurses

void R_init(void);
void R_set(view_t *v);
void R_char(int ch);
void R_string(const char *s);
void R_blueprint(void);
void R_update(void);

#endif /* !__RENDER_H */
