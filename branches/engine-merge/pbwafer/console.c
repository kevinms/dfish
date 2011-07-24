// Copyright (C) 2011  Kevin Smith
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

#include "console.h"

#include <stdio.h>
#include <stdarg.h>

#include "render.h"
#include "input.h"
#include "utils.h"
#include "pbwafer.h"

console_t console;

void CONSOLE_init(const char *s,void (*callback)())
{
	console.name = p_strcpy(s);
	buf_init(&console.text, 1024);
	console.default_callback = callback;

	console.history = list_init();
	console.lines = list_init();
	console.commands = list_init();

	console.curhistory = NULL;
	console.nextlines = NULL;
	console.curlines = &console.lines->tail;
	console.prevlines = NULL;

	//CONSOLE_register_cmd("/kick","%s",PROTO_kick);
	//CONSOLE_register_cmd("/login","%s%s",PROTO_login);
	CONSOLE_register_cmd("/help",0,CONSOLE_help,NULL,"Tells how to use the console");
	CONSOLE_register_cmd("/clear",0,CONSOLE_clear,NULL,"Clear the console");
	CONSOLE_register_cmd("/cmdlist",0,CONSOLE_cmdlist,NULL,"List all the commands");
	CONSOLE_register_cmd("/quit",0,PBWFAER_quit,NULL,"Quit the game");
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
	int i;
	link_t *tmp;

	if(g_chain.uni == K_RETURN || g_chain.uni == K_KP_ENTER) {
		if(console.text.cursize == 0)
			return;
		buf_write_char(&console.text, '\0');
		s = buf_read_string(&console.text);
		buf_reset(&console.text);

		console.curhistory = NULL;
		console.nextlines = NULL;
		console.curlines = &console.lines->tail;
		console.prevlines = NULL;

		CONSOLE_add_to_history(s);

		// Do command
		if(s[0] == '/') {
			CONSOLE_print(s);
			CONSOLE_parse_cmd(s);
		}
		// Or do the default action (chat message usually)
		else {
			if(console.default_callback != NULL)
				console.default_callback(s);
			CONSOLE_update();
		}
	}
	else if(g_chain.uni == K_BACKSPACE) {
		if(console.text.cursize > 0)
			console.text.cursize--;
		CONSOLE_update();
	}
	else if(g_chain.sym == K_UP) {
		if(console.history->len == 0)
			return;
		if(console.curhistory == NULL)
			console.curhistory = console.history->tail;
		else if(console.curhistory->prev != NULL)
			console.curhistory = console.curhistory->prev;

		buf_reset(&console.text);
		buf_write_string(&console.text,(const char *)console.curhistory->item);
		console.text.cursize--;
		CONSOLE_update();
	}
	else if(g_chain.sym == K_DOWN) {
		if(console.curhistory != NULL) {
			if(console.curhistory->next != NULL) {
				console.curhistory = console.curhistory->next;
				buf_reset(&console.text);
				buf_write_string(&console.text,(const char *)console.curhistory->item);
				console.text.cursize--;
			}
			else {
				console.curhistory = NULL;
				buf_reset(&console.text);
			}
		}
		CONSOLE_update();
	}
	else if(g_chain.sym == K_PAGEUP) {
		//TODO: change what items from list_t *lines gets displayed on the screen
		if(console.nextlines != NULL) {
			console.prevlines = console.curlines;
			console.curlines = console.nextlines;
			console.nextlines = NULL;
		}
		CONSOLE_update();
	}
	else if(g_chain.sym == K_PAGEDOWN) {

		printf("------------PAGEDOWN-----------\n");

		if(console.nextlines) printf("nextlines: '%s'\n",(char *)(*console.nextlines)->item);
		if(console.curlines) printf("curlines: '%s'\n",(char *)(*console.curlines)->item);
		if(console.prevlines) printf("prevlines: '%s'\n",(char *)(*console.prevlines)->item);


		//TODO: change what items from list_t *lines gets displayed on the screen
		if(console.prevlines != NULL) {
			// Get the next prevlines
			for(i=1, tmp=(*console.curlines); i < console.v->fake_h-2 && tmp != NULL; tmp=tmp->next) {
				console.prevlines = &tmp;
				i += (strlen((char *)tmp->item) / (console.v->fake_w-1) + 1);
			}

			if(tmp == NULL) {
				console.nextlines = console.curlines;
				console.curlines = &console.lines->tail;
				console.prevlines = NULL;
			} else {
				console.nextlines = console.curlines;
				console.curlines = console.prevlines;
				console.prevlines = NULL;
			}
		}
		CONSOLE_update();
	}
	else if ( g_chain.uni < 0x80 && g_chain.uni > 0 ) {
		buf_write_char(&console.text, (char)g_chain.uni);

		//console.curhistory = NULL;
		console.nextlines = NULL;
		console.curlines = &console.lines->tail;
		console.prevlines = NULL;

		CONSOLE_update();
	}
}

void CONSOLE_update()
{
	int i,j;
	int h;
	int len;
	int span = 0;
	link_t *tmp;

	h = console.v->fake_h;

	R_set(console.v);

	// Render name
	R_clear();
	R_move(0,0);
	R_stringln(console.name);

	// Render lines
	R_move(0,h-2);

	for(i = 0, tmp = *console.curlines; i+span < h-1 && tmp != NULL; i+=span, tmp = tmp->prev) {
		//printf("'%s'\n", (char *)tmp->item);

		len = strlen((char *)tmp->item);
		span = len / (console.v->fake_w-1) + 1;
		//printf("span: %d\n",span);
		R_moveby(0,-(span-1));

		for(j = 0; j < len && i+span < h-1; j += console.v->fake_w-1) {
			//printf("part: '%s'\n",(char *)tmp->item+j);
			console.v->cursor_x = 0;
			R_stringn((char *)tmp->item+j,console.v->fake_w-1);
			R_moveby(0,1);
		}
		R_moveby(0,-(span+1));
	}

	if(tmp != NULL)
		console.nextlines = &tmp->next;
	//else
	//	console.nextlines = NULL;

	// Render text
	R_move(0,h-1);
	R_cstring("]> ",0,0,0,0,255,0);
	R_stringn((const char *)console.text.buf, console.text.cursize);

	// Probably don't want to do this here.  Just one master refresh!
	R_update();
}

void CONSOLE_add_to_history(const char *s)
{
	list_add(console.history, (void *)strdup(s));

	if(console.history->len > CONSOLE_MAX_HISTORY)
		list_del_head(console.history);
}

void CONSOLE_print_no_update(const char *s,...)
{
	char buf[1024];

	va_list va;
	va_start (va, s);
	vsprintf(buf,s,va);
	va_end (va);

	list_add(console.lines, (void *)strdup(buf));

	if(console.lines->len > CONSOLE_MAX_LINES)
		list_del_head(console.lines);
}

void CONSOLE_print(const char *s,...)
{
	char buf[1024];

	va_list va;
	va_start (va, s);
	vsprintf(buf,s,va);
	va_end (va);

	list_add(console.lines, (void *)strdup(buf));

	if(console.lines->len > CONSOLE_MAX_LINES)
		list_del_head(console.lines);

	CONSOLE_update();
}

void CONSOLE_toggle()
{
	if(active_view != console.v)
		VIEW_focus(console.v);
	else
		VIEW_focus(prev_view);

	R_boxext(prev_view,0,0,0);
	R_update();
}

void CONSOLE_clear()
{
	list_del(console.lines);

	console.lines = list_init();

	CONSOLE_update();
}

void CONSOLE_help()
{
	CONSOLE_print_no_update("----------Console Guide----------");
	CONSOLE_print_no_update("` key will toggle between the main view and console view");
	CONSOLE_print_no_update("Any printable key can be typed into the console");
	CONSOLE_print_no_update("Up Arrow Key will go back in the history of commands you typed");
	CONSOLE_print_no_update("Down Arrow Key will go forward in the history of commands you typed");
	CONSOLE_print_no_update("Backspace will delete the most recent typed character");
	CONSOLE_print_no_update("Page Up will scroll up one page of lines");
	CONSOLE_print_no_update("Page Down will scroll down one page of lines");

	CONSOLE_update();
}

void CONSOLE_cmdlist()
{
	link_t *tmp;
	console_cmd_t *cc;

	for(tmp = console.commands->tail; tmp != NULL; tmp = tmp->prev) {
		cc = (console_cmd_t *)tmp->item;
		if(!cc->usage)
			CONSOLE_print_no_update("%s - %s",cc->s,cc->descr);
		else
			CONSOLE_print_no_update("%s %s - %s",cc->s,cc->usage,cc->descr);
	}
	CONSOLE_update();
}

void CONSOLE_register_cmd(const char *s,int argc,void (*callback)(),const char *usage,const char *descr)
{
	console_cmd_t *cc = (console_cmd_t *)malloc(sizeof(*cc));

	cc->s = strdup(s);
	cc->argc = argc;
	cc->callback = callback;
	if(!usage)
		cc->usage = NULL;
	else
		cc->usage = strdup(usage);

	if(!descr)
		cc->descr = NULL;
	else
		cc->descr = strdup(descr);

	list_add(console.commands,(void *)cc);
}

void CONSOLE_parse_cmd(char *s)
{
	link_t *tmp;
	console_cmd_t *cc = NULL;
	char *tok;
	int i;
	char **string_array;

	// Read string until ' '
	tok = strtok(s," ");

	for(tmp = console.commands->tail; tmp != NULL; tmp = tmp->prev) {
		cc = (console_cmd_t *)tmp->item;
		if(!strcmp(tok,cc->s))
			break;
	}
	if(!tmp) {
		CONSOLE_print("ERROR: Unkown command '%s'",s);
		return;
	}

	if(!cc->argc) {
		cc->callback();
		return;
	}

	string_array = (char **)malloc(sizeof(char *)*cc->argc);

	// Read all the args into an array of strings.
	for(i = 0; i < cc->argc; i++) {
		tok = strtok(NULL," ");

		// Not enough args given, print usage
		if(tok == NULL) {
			if(cc->usage != NULL)
				CONSOLE_print("Usage: %s %s",cc->s,cc->usage);
			else
				CONSOLE_print("Usage: %s",cc->s);
			return;
		}

		string_array[i] = strdup(tok);
	}

	cc->callback(cc->argc, string_array);
}
