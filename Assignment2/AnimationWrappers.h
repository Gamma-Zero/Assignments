#include <SDL2/SDL.h>

struct Anim{
	SDL_Rect space;
	int x,y,w,h;
	Anim(){
		space={0,0,0,0};
		x=0; y=0; w=0; h=0;
	}
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

struct Player{
	SDL_Texture* sprites;
	Anim walkanimation;
	Anim shootanimation;
	int ssheeth,ssheetw;
	bool choose;
	bool moving;
	int x,y;
	int curr;
	Player(SDL_Texture* s, int x1, int y1){
		sprites=s;
		SDL_QueryTexture(s,NULL,NULL,&ssheetw,&ssheeth);
                walkanimation.y=10*ssheeth/21;
		walkanimation.w=ssheetw/13;
		walkanimation.h=ssheeth/21;
                shootanimation.y=18*ssheeth/21;
		shootanimation.w=ssheetw/13;
		shootanimation.h=ssheeth/21;
		x=x1;
		y=y1;
		choose=0;
		moving=0;
		curr=2;
	}
	void RenderPlayer(SDL_Renderer* render, SDL_Rect space){
		if (!moving){
			walkanimation.x=0;
		}
		if (!choose){
                            walkanimation.y=(curr+8)*ssheeth/21;
                            walkanimation.update();
                            SDL_RenderCopy(render,sprites,&walkanimation.space,&space);
		} else {
                            shootanimation.y=(curr+16)*ssheeth/21;
                            shootanimation.update();
                            SDL_RenderCopy(render,sprites,&shootanimation.space,&space);
                }
	}
	void passiveAnimate(){
		if (!choose){
                             walkanimation.x=(walkanimation.x+ssheetw/13)%(9*ssheetw/13);
                             shootanimation.x=0;
                } else {
                             shootanimation.x=shootanimation.x+ssheetw/13;
                             walkanimation.x=0;
                             if (shootanimation.x==ssheetw){
                                         choose=0;
                             }
               }
	}
};
