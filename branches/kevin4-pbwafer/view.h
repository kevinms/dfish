+----------+
|    1     |   # of views: 2
|          |
+----------+
|    2     |
+----------+

Each view could have an xoffset and yoffset

extern int TC_X,TC_Y,TC_R,TC_G,TC_B,TC_FR,TC_FG,TC_FB,TC_A,TC_FA,TC_L
extern SDL_Surface *TC_Screen;
extern bool TC_LayersShown[7];
extern Uint32 TC_Layers[255][255][7][7];

typedef struct view_s
{
	int real_w; // Root SDL_Surface width for this view in pixels
	int real_h; // Root SDL_Surface height for this view in pixels

	int tile_w; // width of a single tile
	int tile_h; // height of a single tile

	int fake_w; // width by # of tiles
	int fake_h; // height by # of tiles

	int x; // x position of the view on the ROOT WINDOW SDL_Surface
	int y; // y position of the view on the ROOT WINDOW SDL_Surface

	char *fontname;
	int fontsize;
	TTF_Font *font;
} view_t;

view_t view_init(int x, int y, int real_w, int real_h, int tile_w, int tile_h);
void view_update(view_t *v);
void view_free(view_t *v);

if you want to learn all about how tinycurses works just look into:
	int initscrext(int w,int h,bool real);
	int addch(int ch)
	int refresh();

	Also its vital to figure out what all TC_Layers is used for
