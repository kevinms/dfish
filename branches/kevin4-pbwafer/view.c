#include "view.h"

view_t view_init(int x, int y, int real_w, int real_h, int tile_w, int tile_h, char *fontname, int fontsize)
{
	view_t *v = (view_t *)malloc(sizeof(*v));
	if(!view_t)
		return NULL;

	v->x = x;
	v->y = y;

	v->real_w = real_w;
	v->real_h = real_h;

	v->tile_w = tile_w;
	v->tile_h = tile_h;

	v->fontname = fontname;
	v->fontsize = fontsize;

	v->font = TTF_OpenFont(fontname,fontsize);
	TTF_GlyphMetrics(v->font,'@',&x,&x,&x,&x,&TC_XSpace);
	TC_YSpace = TTF_FontLineSkip(v->font)+1;

	return v;
}

void view_update(view_t *v)
{

}

void view_free(view_t *v)
{

}
