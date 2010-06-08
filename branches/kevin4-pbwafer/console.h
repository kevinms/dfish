#ifndef __CONSOLE_H
#define __CONSOLE_H

//TODO: Be able to accept input from the user, need to some how pass the input
//      to this view.

#include "list.h"
#include "buf.h"
#include "view.h"

#define CONSOLE_MAX_HISTORY 4
#define CONSOLE_MAX_LINES   4

typedef struct console_s
{
	const char *name;
	list_t *history;
	list_t *lines;
	fixedbuf_t text;

	link_t *curhistory;
	link_t *curlines;

	view_t *v;
} console_t;

extern console_t console;

void CONSOLE_init(const char *s);
void CONSOLE_attach_to_view(view_t *v);
void CONSOLE_accept_input();
void CONSOLE_update();
void CONSOLE_add_to_history(const char *s);
void CONSOLE_print(const char *s);

#endif /* !__CONSOLE_H */
