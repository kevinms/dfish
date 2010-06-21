#ifndef __CONSOLE_H
#define __CONSOLE_H

//TODO: Fix PAGEUP and PAGEDOWN events
//TODO: Implement parsing of arguments for commands in CONSOLE_parse_cmd()
//TODO: Change most printf statements in proto to CONSOLE_print()'s
//TODO: Need to have a debug option in CONSOLE_print() that will print everything
//      to the terminal as well.  Only if DBGPRINT is enabled (or something)

#include "list.h"
#include "buf.h"
#include "view.h"

#define CONSOLE_MAX_HISTORY 70
#define CONSOLE_MAX_LINES   70

typedef struct console_s
{
	const char *name;
	list_t *history;
	list_t *lines;
	list_t *commands;
	fixedbuf_t text;

	link_t *curhistory;
	link_t **nextlines;
	link_t **prevlines;
	link_t **curlines;

	void (*default_callback)();

	view_t *v;
} console_t;

typedef struct console_cmd_s
{
	const char *s;
	int argc;
	void (*callback)();
	const char *usage;
} console_cmd_t;

extern console_t console;

void CONSOLE_init(const char *s,void (*callback)());
void CONSOLE_attach_to_view(view_t *v);
void CONSOLE_accept_input();
void CONSOLE_update();
void CONSOLE_add_to_history(const char *s);
void CONSOLE_print_no_update(const char *s,...);
void CONSOLE_print(const char *s,...);
void CONSOLE_print_err(const char *s,...);
void CONSOLE_toggle();
void CONSOLE_clear();
void CONSOLE_help();

void CONSOLE_register_cmd(const char *s,int argc,void (*callback)(),const char *usage);
void CONSOLE_parse_cmd(char *s);

//TODO: Dump console lines to a file, maybe also dump the console history
//      two seperate commands maybe?
void CONSOLE_dump(const char *s);

#endif /* !__CONSOLE_H */
