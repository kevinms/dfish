#ifndef __RENDER_H
#define __RENDER_H

#include "view.h"

//TODO: render the cursor

#define CLAMP(x) if((x) < 0) (x) = 0; else if((x) > 255) (x) = 255;

//TODO: Implement robust blueprint rendering
void R_blueprint(void);

// Initialize render module and set view
void R_init(void);
void R_set(view_t *v);

// Cursor rendering options
void R_setcursorlook(char r,char g,char b);
void R_showcursor(char b);

// Basic render functions
void R_setch(int ch);
void R_addch(int ch);
void R_string(const char *s);
void R_move(int x,int y);
void R_moveby(int x,int y);
void R_updatelayer(int l);
void R_update(void);
void R_updateall(void);
void R_clearlayer(int l);
void R_clear(void);
void R_color(char r,char g,char b,char fr,char fg,char fb);

// Change color then render
void R_csetch(int ch,char r,char g,char b,char fr,char fg,char fb);
void R_caddch(int ch,char r,char g,char b,char fr,char fg,char fb);
void R_cstring(const char *s,char r,char g,char b,char fr,char fg,char fb);

// Move cursor then render
void R_mvsetch(int ch,int x,int y);
void R_mvaddch(int ch,int x,int y);
void R_mvstring(const char *s,int x,int y);
void R_mvclearch(int l,int x,int y);

// Move and change color then render
void R_setchext(int ch,int x,int y,char r,char g,char b,char fr,char fg,char fb);
void R_addchext(int ch,int x,int y,char r,char g,char b,char fr,char fg,char fb);
void R_stringext(const char *s,int x,int y,char r,char g,char b,char fr,char fg,char fb);
void R_clearlayerext(int ch,char r,char g,char b,char fr,char fg,char fb, int l);
void R_clearext(int ch,char r,char g,char b,char fr,char fg,char fb);

// Advanced functions
void R_setalpha(char a);
void R_setlayer(char l);
void R_setlayershown(char l,char s);
void R_setcolor(int x,int y,char r,char g,char b,char fr,char fg,char fb);
void R_addcolor(int x,int y,int r,int g,int b,int fr,int fg,int fb);

#endif /* !__RENDER_H */
