#include "render.h"

#include "SDL_ttf.h"

view_t *v = NULL;

void R_init(void)
{
	TTF_Init();
	SDL_EnableUNICODE(1);
}

void R_set(view_t *view)
{
	v = view;
}

void R_char(int ch)
{
	v->layers[v->cursor_x][v->cursor_y][0][0] = ch;
	v->layers[v->cursor_x][v->cursor_y][0][1] = SDL_MapRGBA(v->screen->format,255,0,0,255);
	v->layers[v->cursor_x][v->cursor_y][0][2] = SDL_MapRGBA(v->screen->format,0,255,0,255);
	v->layers[v->cursor_x][v->cursor_y][0][3] = 255;

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
		R_char(s[i]);
}

void R_blueprint(void)
{

}

void R_update(void)
{
	int i,j,k;
	Uint8 a;
	SDL_Surface *s,*t;
	SDL_Rect r;
	SDL_Color c;
	Uint16 txt[2] = {'\0','\0'};

	printf("fake_w=%d\nfake_h=%d\n",v->fake_w,v->fake_h);

	for(i = 0; i < v->fake_w; i++) {
		for(j = 0; j < v->fake_h; j++) {
			r.x = i*v->tile_w + v->x;
			r.y = j*v->tile_h + v->y;

			txt[0] = v->layers[i][j][0][0];
			if(txt[0]=='\0'&&txt[1]=='\0')
				txt[0]=' ';

			SDL_GetRGBA(v->layers[i][j][0][2],v->screen->format,&c.r,&c.g,&c.b,&a);
			t=TTF_RenderUNICODE_Blended(v->font,txt,c);

			s=SDL_DisplayFormat(t);
			SDL_FillRect(s,NULL,v->layers[i][j][0][1]);
			SDL_BlitSurface(t,NULL,s,NULL);

			SDL_BlitSurface(s,NULL,v->screen,&r);

			SDL_FreeSurface(s);
			SDL_FreeSurface(t);
		}
	}

	SDL_Flip(v->screen);
}
