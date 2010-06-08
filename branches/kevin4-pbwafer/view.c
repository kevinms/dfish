#include "view.h"
#include "utils.h"

view_t *active_view;

view_t *VIEW_init(int x, int y, int real_w, int real_h, char *fontname, int fontsize, SDL_Surface *screen, int numl)
{
	view_t *v = (view_t *)malloc(sizeof(*v));
	if(!v)
		return NULL;

	v->x = x;
	v->y = y;

	v->real_w = real_w;
	v->real_h = real_h;

	v->screen = screen;

	v->fontname = p_strcpy(fontname);
	v->fontsize = fontsize;

	v->font = TTF_OpenFont(v->fontname,v->fontsize);

	// Get the tile_w and tile_h
	TTF_GlyphMetrics(v->font,'@',&x,&x,&x,&x,&v->tile_w);
	v->tile_h = TTF_FontLineSkip(v->font)+1;

	// Calculate the fake_w and fake_h
	v->fake_w = v->real_w / v->tile_w;
	v->fake_h = v->real_h / v->tile_h;

	// Set the cursor position to the top left
	v->cursor_x = 0;
	v->cursor_y = 0;

	// Get all the layers ready
	v->numl = numl;
	v->layer_visible = (char *)malloc(sizeof(char) * numl);
	v->l = 0;
	VIEW_layers_malloc(v);

	return v;
}

void VIEW_layers_malloc(view_t *v)
{
	// Allocate room for all the chars in this view (4D array)
	int i,j,k,size;
	size = sizeof(v->layers);
	v->layers = (Uint32 ****)malloc(size * v->fake_w);
	for(i=0;i<v->fake_w;i++) {
		v->layers[i] = (Uint32 ***)malloc(size * v->fake_h);
		for(j=0;j<v->fake_h;j++) {
			v->layers[i][j] = (Uint32 **)malloc(size * v->numl);
			for(k=0;k<v->numl;k++) {
				v->layers[i][j][k] = (Uint32 *)malloc(sizeof(Uint32) * 7);
			}
		}
	}
}

void VIEW_resize(view_t *v)
{

}

void VIEW_update(view_t *v)
{

}

void VIEW_free(view_t *v)
{

}
