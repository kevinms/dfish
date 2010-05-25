#include "render.h"

#include "SDL.h"
#include "SDL_ttf.h"
#include <stdio.h>

view_t *v = NULL;
SDL_Color g_fg = {255,0,0,255};
SDL_Color g_bg = {0,0,0,255};

void R_init(void)
{
	TTF_Init();
	SDL_EnableUNICODE(1);
}

void R_set(view_t *view)
{
	v = view;
}

void R_setcursorlook(char r,char g,char b)
{
	v->cursor_color[0] = r;
	v->cursor_color[1] = g;
	v->cursor_color[2] = b;
}

void R_showcursor(char b)
{
	v->showcursor = b;
}

void R_setch(int ch)
{
	v->layers[v->cursor_x][v->cursor_y][v->l][0] = ch;
	v->layers[v->cursor_x][v->cursor_y][v->l][1] = SDL_MapRGBA(v->screen->format,g_bg.r,g_bg.g,g_bg.b,g_bg.unused);
	v->layers[v->cursor_x][v->cursor_y][v->l][2] = SDL_MapRGBA(v->screen->format,g_fg.r,g_fg.g,g_fg.b,g_fg.unused);
	v->layers[v->cursor_x][v->cursor_y][v->l][3] = g_fg.unused;
}

void R_addch(int ch)
{
	R_setch(ch);

	if((++v->cursor_x) > v->fake_w-1) {
		v->cursor_x = 0;
		if((++v->cursor_y) > v->fake_h-1)
			v->cursor_y = v->fake_h-1;
	}
}

void R_string(const char *s)
{
	int i;
	int len = strlen(s);
	for(i = 0; i < len; i++)
		R_addch(s[i]);
}

void R_stringln(const char *s)
{
	R_string(s);
	v->cursor_y++;
	v->cursor_x = 0;
}

void R_move(int x, int y)
{
	if(0 >= x && x < v->fake_w) {
		if(0 >= y && y < v->fake_h) {
			v->cursor_x = x;
			v->cursor_y = y;
		}
	}
}

void R_moveby(int x,int y)
{
	x += v->cursor_x;
	y += v->cursor_y;
	if(0 >= x && x < v->fake_w) {
		if(0 >= y && y < v->fake_h) {
			v->cursor_x = x;
			v->cursor_y = y;
		}
	}
}

void R_updatelayer(int l)
{
	int i,j;
	Uint8 a;
	SDL_Surface *s,*t;
	SDL_Rect r;
	SDL_Color c;
	Uint16 txt[2] = {'\0','\0'};

	printf("fake_w=%d\nfake_h=%d\n",v->fake_w,v->fake_h);

	if(l<0||l>=v->numl) {
		fprintf(stderr,"Bad layer: %d\n",l);
		return;
	}

	if(!v->layer_visible[l])
		return;

	for(i = 0; i < v->fake_w; i++) {
		for(j = 0; j < v->fake_h; j++) {
			r.x = i*v->tile_w + v->x;
			r.y = j*v->tile_h + v->y;

			txt[0] = v->layers[i][j][l][0];
			if(txt[0]=='\0'&&txt[1]=='\0')
				txt[0]=' ';

			SDL_GetRGBA(v->layers[i][j][l][2],v->screen->format,&c.r,&c.g,&c.b,&a);
			t=TTF_RenderUNICODE_Blended(v->font,txt,c);

			s=SDL_DisplayFormat(t);
			SDL_FillRect(s,NULL,v->layers[i][j][l][1]);
			SDL_BlitSurface(t,NULL,s,NULL);

			SDL_BlitSurface(s,NULL,v->screen,&r);

			SDL_FreeSurface(s);
			SDL_FreeSurface(t);
		}
	}

	SDL_Flip(v->screen);
}

void R_update(void)
{
	R_updatelayer(v->l);
}

void R_updateall(void)
{
	int l;
	for(l=0;l<v->numl;l++)
		R_updatelayer(l);
}

void R_clearlayer(int l)
{
	int i,j;
	char tmp = g_fg.unused;
	char tmpl = v->l;
	R_setlayer(l);
	R_setalpha(0);

	for(i=0; i < v->fake_w; i++)
		for(j=0; j < v->fake_h; j++)
			R_mvsetch(' ',i,j);

	R_setalpha(tmp);
	R_setlayer(tmpl);
}

void R_clear(void)
{
	int i;
	for(i=0; i<v->numl; i++)
		R_clearlayer(i);
}

void R_color(char r,char g,char b,char fr,char fg,char fb)
{
	g_bg.r = r;
	g_bg.g = g;
	g_bg.b = b;

	g_fg.r = fr;
	g_fg.g = fg;
	g_fg.b = fb;
}

void R_csetch(int ch,char r,char g,char b,char fr,char fg,char fb)
{
	v->layers[v->cursor_x][v->cursor_y][v->l][0] = ch;
	v->layers[v->cursor_x][v->cursor_y][v->l][1] = SDL_MapRGBA(v->screen->format,r,g,b,g_bg.unused);
	v->layers[v->cursor_x][v->cursor_y][v->l][2] = SDL_MapRGBA(v->screen->format,fr,fg,fb,g_fg.unused);
	v->layers[v->cursor_x][v->cursor_y][v->l][3] = g_fg.unused;
}

void R_caddch(int ch,char r,char g,char b,char fr,char fg,char fb)
{
	R_csetch(ch,r,g,b,fr,fg,fb);

	if((++v->cursor_x) > v->fake_w-1) {
		v->cursor_x = 0;
		if((++v->cursor_y) > v->fake_h-1)
			v->cursor_y = v->fake_h-1;
	}
}

void R_cstring(const char *s,char r,char g,char b,char fr,char fg,char fb)
{
	int i;
	int len = strlen(s);
	for(i = 0; i < len; i++)
		R_caddch(s[i],r,g,b,fr,fg,fb);
}

void R_mvsetch(int ch,int x,int y)
{
	if(0 <= x && x < v->fake_w) {
		if(0 <= y && y < v->fake_h) {
			v->layers[x][y][v->l][0] = ch;
			v->layers[x][y][v->l][1] = SDL_MapRGBA(v->screen->format,g_bg.r,g_bg.g,g_bg.b,g_bg.unused);
			v->layers[x][y][v->l][2] = SDL_MapRGBA(v->screen->format,g_fg.r,g_fg.g,g_fg.b,g_fg.unused);
			v->layers[x][y][v->l][3] = g_fg.unused;
		}
	}
}

void R_mvaddch(int ch,int x,int y)
{
	R_move(x,y);
	R_addch(ch);
}

void R_mvstring(const char *s,int x,int y)
{
	R_move(x,y);
	R_string(s);
}

void R_mvclearch(int l,int x,int y)
{
	char tmp = g_bg.unused;
	R_setalpha(0);
	R_mvsetch(' ',x,y);
	R_setalpha(tmp);
}

void R_setchext(int ch,int x,int y,char r,char g,char b,char fr,char fg,char fb)
{
	if(0 <= x && x < v->fake_w) {
		if(0 <= y && y < v->fake_h) {
			v->layers[x][y][v->l][0] = ch;
			v->layers[x][y][v->l][1] = SDL_MapRGBA(v->screen->format,r,g,b,g_bg.unused);
			v->layers[x][y][v->l][2] = SDL_MapRGBA(v->screen->format,fr,fg,fb,g_fg.unused);
			v->layers[x][y][v->l][3] = g_fg.unused;
		}
	}
}

void R_addchext(int ch,int x,int y,char r,char g,char b,char fr,char fg,char fb)
{
	R_move(x,y);
	R_caddch(ch,r,g,b,fr,fg,fb);
}

void R_stringext(const char *s,int x,int y,char r,char g,char b,char fr,char fg,char fb)
{
	R_move(x,y);
	R_cstring(s,r,g,b,fr,fg,fb);
}

void R_clearlayerext(int ch,char r,char g,char b,char fr,char fg,char fb, int l)
{
	int i,j;
	char tmp = g_fg.unused;
	char tmpl = v->l;
	R_setlayer(l);
	R_setalpha(0);

	for(i=0; i < v->fake_w; i++)
		for(j=0; j < v->fake_h; j++)
			R_setchext(' ',i,j,r,g,b,fr,fg,fb);

	R_setalpha(tmp);
	R_setlayer(tmpl);
}

void R_clearext(int ch,char r,char g,char b,char fr,char fg,char fb)
{
	int l;
	for(l=0; l < v->numl; l++) {
		R_clearlayerext(ch,r,g,b,fr,fg,fb,l);
	}
}

void R_setalpha(char a)
{
	g_bg.unused = a;
	g_fg.unused = a;
	CLAMP(g_bg.unused);
	CLAMP(g_fg.unused);
}

void R_setlayer(char l)
{
	v->l = l;
}

void R_setlayershown(char l,char s)
{
	if(0 <= l && l < v->numl)
		v->layer_visible[v->l] = s;
}

void R_setcolor(int x,int y,char r,char g,char b,char fr,char fg,char fb)
{
	if(0 >= x && x < v->fake_w) {
		if(0 >= y && y < v->fake_h) {
			v->layers[x][y][v->l][1] = SDL_MapRGBA(v->screen->format,r,g,b,g_bg.unused);
			v->layers[x][y][v->l][2] = SDL_MapRGBA(v->screen->format,fr,fg,fb,g_fg.unused);
		}
	}
}

void R_addcolor(int x,int y,int r,int g,int b,int fr,int fg,int fb)
{
	Uint8 fc[4],bc[4];
	if(0 >= x && x < v->fake_w) {
		if(0 >= y && y < v->fake_h) {
			SDL_GetRGBA(v->layers[x][y][v->l][1],v->screen->format,&bc[0],&bc[1],&bc[2],&bc[3]);
			SDL_GetRGBA(v->layers[x][y][v->l][2],v->screen->format,&fc[0],&fc[1],&fc[2],&fc[3]);

			r += bc[0];
			g += bc[1];
			b += bc[2];
			CLAMP(r);
			CLAMP(g);
			CLAMP(b);

			fr += fc[0];
			fg += fc[1];
			fb += fc[2];
			CLAMP(fr);
			CLAMP(fg);
			CLAMP(fb);

			v->layers[x][y][v->l][1] = SDL_MapRGBA(v->screen->format,r,g,b,bc[3]);
			v->layers[x][y][v->l][2] = SDL_MapRGBA(v->screen->format,fr,fg,fb,fc[3]);
		}
	}
}
