#include <SDL2/SDL.h>
#include "spawnnew.h"

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
	Anim bombanimation;
	int ssheeth,ssheetw;
	int choose;
	bool moving;
	int x,y;
	int curr;
	int HP=100;
	Player(SDL_Texture* s, int x1, int y1){
		sprites=s;
		SDL_QueryTexture(s,NULL,NULL,&ssheetw,&ssheeth);
                walkanimation.y=10*ssheeth/21;
		walkanimation.w=ssheetw/13;
		walkanimation.h=ssheeth/21;
                shootanimation.y=18*ssheeth/21;
		shootanimation.w=ssheetw/13;
		shootanimation.h=ssheeth/21;
		bombanimation.y=2*ssheetw/13;
                bombanimation.w=ssheetw/13;
                bombanimation.h=ssheeth/21;
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
		if (choose==0){
                            walkanimation.y=(curr+8)*ssheeth/21;
                            walkanimation.update();
                            SDL_RenderCopy(render,sprites,&walkanimation.space,&space);
		} else if (choose==1){
                            shootanimation.y=(curr+16)*ssheeth/21;
                            shootanimation.update();
                            SDL_RenderCopy(render,sprites,&shootanimation.space,&space);
                } else if (choose=2){
			    bombanimation.y=(curr)*ssheeth/21;
                            bombanimation.update();
                            SDL_RenderCopy(render,sprites,&bombanimation.space,&space);
		}

	}
	void passiveAnimate(){
		if (choose==0){
                             walkanimation.x=(walkanimation.x+ssheetw/13)%(9*ssheetw/13);
                             shootanimation.x=0;
			     bombanimation.x=0;
                } else if (choose==1){
                             shootanimation.x=shootanimation.x+ssheetw/13;
                             walkanimation.x=0;
                             if (shootanimation.x==ssheetw){
                                         choose=0;
					 shootanimation.x=0;
                             }
               } else if (choose==2){
		       bombanimation.x=bombanimation.x+ssheetw/13;
                       walkanimation.x=0;
                       if (bombanimation.x==7*ssheetw/13){
                                         choose=0;
                                         bombanimation.x=0;
                        }
               }
	}
};

struct Enemy{
	SDL_Texture* sprites;
	Anim walkanimation;
	int ssheeth,ssheetw;
	pair<int,int> locations;
	int dir;
	int HP=20;
	int SPRITE=40;
	Enemy(SDL_Texture* s,pair<int,int> loc){
		sprites=s;
		SDL_QueryTexture(s,NULL,NULL,&ssheetw,&ssheeth);
		walkanimation=Anim(0,10*ssheeth/21,ssheetw/13,ssheeth/21);
		locations=loc;
		dir=-1;
	}
	void RenderEnemy(int x1, int y1, SDL_Renderer* render){
			walkanimation.x=(walkanimation.x+ssheetw/13)%(9*ssheetw/13);
			walkanimation.y=(dir+8)*ssheeth/21;
                        walkanimation.update();
			SDL_Rect space={x1,y1,SPRITE,SPRITE};
			SDL_RenderCopy(render,sprites,&walkanimation.space,&space);
	}
};

int finddir(pair<int,int> s, pair<int,int> e){
                if (e.first==s.first-1){
                        return 1;
                } else if (e.first==s.first+1){
                        return 3;
                } else if (e.second==s.second-1){
                        return 0;
                } else {
                        return 2;
                }
        }
