#ifndef __VIEW_CONSOLE_H
#define __VIEW_CONSOLE_H

#include "pbwafer/view.h"

extern view_t *v_console;

void VCONSOLE_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl);

// Console command callbacks
void CB_req_servinfo_ip(int count, const char **s);
void CB_req_servinfo_broadcast(int count, const char **s);
void CB_netsim(int count, const char **s);

#endif /* !__VIEW_CONSOLE_H */
