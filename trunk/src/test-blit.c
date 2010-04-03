/********************************************************
 * test-blit.c
 * 
 * Tests SDL blitting stuff
 * 
 ********************************************************/

#include <assert.h>
#include "SDL.h"
#include "TinyCurses.h"
#include "keys.h"
#include "system.h"
#include "map.h"

int main (int argc, char *argv[]) {
	
	char *lps = "lps";
	printf("asf");
	/* Initializes local and global map */
	struct posSys_t *GPS, *LPS;
	GPS = init_map(lGDefault, wGDefault, argv[1]);
	LPS = init_map(lLDefault, wLDefault, lps);

	//TODO: Have a struct lgroup and generate N solarsystems
	/* Here are two different ways to generate a solar system */
	struct system_t *solSys;
	//solSys = system_init_file("dataz/obj_data.dat", GPS);
	solSys = system_init_procedural(GPS);
	
	LPS = fix_map(GPS, LPS);
	int i,j;
	initscrext(wLDefault, lLDefault, false);

	refresh();
	for(i = 0; i < LPS->lSize; i++) {
		for(j = 0; j < LPS->wSize; j++){
			//printw("%c", LPS->map[i][j]);
			addchext(LPS->map[i][j],j,i,0,0,0,0,255,0);
		}
	}
	move(0,0);
	refresh();

	/* Title */
	SDL_WM_SetCaption( "Dfish", NULL );
	
	
///Blitting pseudo-code (tinycurses)
///make a new UInt32 TCLayers[255][255][7][7] array to hold other interface windows
Uint32 TC_Layers[255][255][7][7];
///		states 2 SDLsurface ptrs s and t, 1 SDLrect r and SDLcolor
Uint8 a;
SDL_Surface *s, *t;
SDL_Rect r;
SDL_Color c;
Uint16 txt[2] = {'\0','\0'};
///		sets r.h to TCfontspace height and r.w ...
TC_XSpace = FONT_SPACE_WIDTH;
TC_YSpace = FONT_SPACE_HEIGHT;
r.w = TC_XSpace;
r.h = TC_YSpace;
///		for i<TCwidth j<TCheight and k<7 (Layer term)
///			if the layer is shown
int k = 0;
for(i=200;i<255;i++)for(j=200;j<255;j++) {
	TC_Layers[i][j][k][0] = 'j';
	TC_Layers[i][j][k][1] = SDL_MapRGBA(TC_Screen->format,TC_R,TC_G,TC_B,TC_A);
	TC_Layers[i][j][k][2] = SDL_MapRGBA(TC_Screen->format,TC_FR,TC_FG,TC_FB,TC_FA);
	TC_Layers[i][j][k][3] = TC_FA;
      r.x = i*TC_XSpace;
      r.y = j*TC_YSpace;
      //need pixel format
      SDL_GetRGBA(TC_Layers[i][j][k][2],TC_Screen->format,&c.r,&c.g,&c.b,&a);
      txt[0] = '!';
      if(txt[0]=='\0'&&txt[1]=='\0')
	txt[0]=' ';
      t=TTF_RenderUNICODE_Blended(TC_Font,txt,c);
      s=SDL_DisplayFormat(t);
//      SDL_FillRect(s,NULL,TC_Layers[i][j][k][1]);
      SDL_BlitSurface(t,NULL,s,NULL);
      if(k!=0)
	SDL_SetAlpha(s,SDL_SRCALPHA,TC_Layers[i][j][k][3]);
      if(k==0||TC_Layers[i][j][k][0]!='\0')
	SDL_BlitSurface(s,NULL,TC_Screen,&r);
      SDL_FreeSurface(s);
      SDL_FreeSurface(t);
    };
///				sets r.x to font width and r.y to font height
///				SDLgetrgba for TClayers[i][j][k][2](pixel) TCscreen->format put into SDLcolor ptr
///				t = SDLrenderunicode blended in TCfont for txt with SDLcolor ptr
///				s = SDLdisplayformat t, converts t to current display format
///				fills s with TClayers[ijk][1]
///				fast blits t onto s
///				fast blits s onto TCscreen at rect r
///				SDLfreesurface s and t
	
	
	SDL_Event test_event;
	while(1){

/************************************************************
 * 
 * Put in normal operating functions like AI paths and stuff
 * This is the program daemon, it will run until we kill
 * 		it within the functions of the program
 * 
*************************************************************/
		/* Checks for events posted into the queue */
		if (SDL_PollEvent(&test_event)) {
			switch (test_event.type) {
				case SDL_KEYDOWN:
					downKeyEvent(&test_event, GPS, LPS);
					break;

				/* Exits with terminal message "Reality Was Aborted" when the 
				 * (x) button is clicked or the program is sent an alt + f4 
				 * signal */
				case SDL_QUIT:
					printf("Reality Was ");
					abort();
					break;
				default: 
					break;
			}
		}
	}

	return 0;
}
