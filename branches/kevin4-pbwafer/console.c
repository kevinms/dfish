#include "console.h"

#include <stdio.h>

#include "render.h"
#include "input.h"
#include "utils.h"

console_t console;

void CONSOLE_init(const char *s)
{
	console.name = p_strcpy(s);
	buf_init(&console.text, 1024);
	console.history = list_init();
	console.lines = list_init();
}

void CONSOLE_attach_to_view(view_t *v)
{
	console.v = v;
	v->accept_input = CONSOLE_accept_input;
	v->update = CONSOLE_update;
}

void CONSOLE_accept_input()
{
	char *s;

	if(g_chain.sym == K_RETURN || g_chain.sym == K_KP_ENTER) {
		buf_write_char(&console.text, '\0');
		s = buf_read_string(&console.text);
		CONSOLE_print(s);
		CONSOLE_add_to_history(p_strcpy(s));
		buf_reset(&console.text);
		//TODO: do command
	}
	else if(g_chain.sym == K_BACKSPACE) {
		console.text.cursize--;
	}
	else if(g_chain.sym == K_UP) {
		buf_reset(&console.text);
		//buf_write_string(
	}
	else
		buf_write_char(&console.text, g_chain.sym);

	CONSOLE_update();
}

void CONSOLE_update()
{
	int i;
	int h;
	link_t *tmp;

	h = console.v->fake_h;

	R_set(console.v);

	// Render name
	R_clear();
	R_move(0,0);
	R_stringln(console.name);

	// Render text
	R_move(0,console.v->fake_h-1);
	R_stringn((const char *)console.text.buf, console.text.cursize);

	// Render lines
	for(i = 0, tmp = console.lines->tail; i < h-2 && tmp != NULL; i++, tmp = tmp->prev) {
		console.v->cursor_x = 0;
		R_moveby(0,-1);
		printf("'%s'\n", (char *)tmp->item);
		R_string((char *)tmp->item);
	}

	// Probably don't want to do this here.  Just one master refresh!
	R_update();
}

void CONSOLE_add_to_history(const char *s)
{
	list_add(console.history, (void *)p_strcpy(s));

	if(console.history->len > CONSOLE_MAX_HISTORY)
		list_del_head(console.history);
}

void CONSOLE_print(const char *s)
{
	printf("consoleprint:'%s'\n",s);
	list_add(console.lines, (void *)p_strcpy(s));

	if(console.lines->len > CONSOLE_MAX_LINES)
		list_del_head(console.lines);

	CONSOLE_update();
}
