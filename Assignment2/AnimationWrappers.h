#include <SDL2/SDL.h>

struct Anim{
	SDL_Rect space;
	int x,y,w,h;
	Anim(int x1, int y1, int w1, int h1){
		space={x1,y1,w1,h1};
		x=x1;
		y=y1;
		w=w1;
		h=h1;
	}
	void update(){
		space={x,y,w,h};
	}
};
