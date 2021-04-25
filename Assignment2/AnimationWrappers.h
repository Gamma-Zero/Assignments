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
	vector<Anim> walkanimation;
	int ssheeth,ssheetw;
	vector<pair<int,int>> locations;
	vector<int> dirs;
	int SPRITE=40;
	Enemy(SDL_Texture* s,vector<pair<int,int>> loc){
		sprites=s;
		SDL_QueryTexture(s,NULL,NULL,&ssheetw,&ssheeth);
		for(auto i:loc){
			Anim temp=Anim(0,10*ssheeth/21,ssheetw/13,ssheeth/21);
			walkanimation.push_back(temp);
		}
		locations=loc;
		for(auto i:loc){
			dirs.push_back(-1);
		}
	}
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
	void RenderEnemy(int x1, int y1, int x2, int y2, SDL_Renderer* render){
		vector<pair<int,int>> temp=move(y1,x1,y2,x2,locations);
		for(int i=0;i<temp.size();++i){
			int curr=finddir(locations[i],temp[i]);
			dirs[i]=curr;
			walkanimation[i].x=(walkanimation[i].x+ssheetw/13)%(9*ssheetw/13);
			walkanimation[i].y=(curr+8)*ssheeth/21;
                        walkanimation[i].update();
			SDL_Rect space={temp[i].first,temp[i].second,SPRITE,SPRITE};
			SDL_RenderCopy(render,sprites,&walkanimation[i].space,&space);
		}
		locations=temp;
	}
	void AddEnemy(pair<int,int> loc){
		Anim temp=Anim(0,10*ssheeth/21,ssheetw/13,ssheeth/21);
                walkanimation.push_back(temp);
		locations.push_back(loc);
	}
};
