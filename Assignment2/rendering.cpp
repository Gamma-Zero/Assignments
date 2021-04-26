#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
#include "projectiles.h"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
const int SPRITE = 40;
const int FPS = 60;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* render=NULL;
SDL_Texture* wall=NULL;
vector<vector<bool>> maze;
vector<Bomb> bombs;
vector<Enemy> en;
vector<int> ehit;
int frame=0;
int eid=0;
int schedule=-1;

bool init(){
	bool success=true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
                printf( "SDL died: %s\n", SDL_GetError() );
		success=false;
        }
        else
        {
                window = SDL_CreateWindow( "Darkest Curse of the Dead Cells", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
                if( window == NULL )
                {
                printf( "Window died: %s\n", SDL_GetError() );
		success=false;
                }
                else
                {
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags)){
				printf("SDL_Image died\n");
				success=false;
			} else {
				render=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (render==NULL){
					printf("Renderer died\n");
					success=false;
				} else {
				        maze=MazeGenerate();
					SDL_SetRenderDrawColor(render,0xFF,0xFF,0xFF,0xFF);
					screenSurface=SDL_GetWindowSurface(window);
				}
			}
                }
        }
	return success;
}

SDL_Surface* loadBMP(string path){
	SDL_Surface* temp = SDL_LoadBMP(path.c_str());
	if (temp==NULL){
		printf("Load failed\n");
	}
	return temp;
}

SDL_Surface* loadPNG(string path){
	SDL_Surface* opt=NULL;
	SDL_Surface* temp=IMG_Load(path.c_str());
	if (temp==NULL){
		printf("PNG Load failed\n");
	} else {
		opt=SDL_ConvertSurface(temp,screenSurface->format,0);
		if (opt==NULL){
			printf("Could not optimize\n");
		}
		SDL_FreeSurface(temp);
	}
	return opt;
}

SDL_Texture* loadTexture(SDL_Surface* input){
	Uint32 colorkey=SDL_MapRGB(input->format,0,0,0);
	SDL_SetColorKey(input,SDL_TRUE,colorkey);
	SDL_Texture* temp=SDL_CreateTextureFromSurface(render,input);
	if (temp==NULL){
		printf("Texture Conversion died\n");
	}
	return temp;
}

void close(){
	SDL_DestroyWindow(window);
	window=NULL;
	SDL_DestroyRenderer(render);
	render=NULL;
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[]){
	if (!init()){
		printf("Failed\n");
	} else {
		SDL_Texture* gexp=loadTexture(loadPNG("Textures/grass.png"));
		if (gexp==NULL){
			printf("Failed\n");
		} else {
			bool quit=false;
			SDL_Event e;
			SDL_RenderClear(render);
			vector<pair<int,int>> loc=randspawn(40,40,920,920);
			for (auto i:loc){
				eid++;
				en.push_back(Enemy(eid,loadTexture(loadPNG("Textures/enemy.png")),i));
			}
			Player p1=Player(loadTexture(loadPNG("Textures/p1.png")),40,40);
			Player p2=Player(loadTexture(loadPNG("Textures/p2.png")),920,920);
			SDL_Texture* bombidle=loadTexture(loadPNG("Textures/bomb.png"));
			SDL_Texture* bombexp=loadTexture(loadPNG("Textures/bombexpnew.png"));
			wall=loadTexture(loadPNG("Textures/wall.png"));
			while(!quit){
				cout << schedule << '\n';
				SDL_RenderCopy(render,gexp,NULL,NULL);
                        	for (int i=0;i<maze.size();i++){
                                        for (int j=0;j<maze.size();j++){
                                                if (!maze[i][j]){
                                                        SDL_Rect t={i*SPRITE,j*SPRITE,SPRITE,SPRITE};
                                                        SDL_RenderCopy(render,wall,NULL,&t);
                                                }
                                        }
                                }
				frame++;
				if (schedule!=-1){
					schedule--;
					if (schedule==0){
						eid++;
                                        	en.push_back(Enemy(eid,loadTexture(loadPNG("Textures/enemy.png")),pspawn(p1.x,p1.y,p2.x,p2.y,loc)));
					}
				}
				if(frame==360){
					frame=0;
					eid++;
					en.push_back(Enemy(eid,loadTexture(loadPNG("Textures/enemy.png")),pspawn(p1.x,p1.y,p2.x,p2.y,loc)));
				}
				int store[4]={p1.x,p1.y,p2.x,p2.y};
				p1.passiveAnimate();
				p2.passiveAnimate();
				SDL_PumpEvents();
				const Uint8* keystate=SDL_GetKeyboardState(NULL);
				if (keystate[SDL_SCANCODE_UP]){
					if (p1.choose==0){
						if (p1.curr==0){
						
							p1.y=p1.y-SPRITE/2; p1.moving=1;
                                                } else {
                                                        p1.curr=0;
                                                }
					}
				} else if (keystate[SDL_SCANCODE_DOWN]){
					if (p1.choose==0){
						if (p1.curr==2){
                                                        p1.y=p1.y+SPRITE/2; p1.moving=1;
                                                } else {
                                                        p1.curr=2;
                                                }
					}
				} else if (keystate[SDL_SCANCODE_LEFT]){
					if (p1.choose==0){
						if (p1.curr==1){
                                                        p1.x=p1.x-SPRITE/2; p1.moving=1;
                                                } else {
                                                        p1.curr=1;
                                                }
					}
				} else if (keystate[SDL_SCANCODE_RIGHT]){
					if (p1.choose==0){
						if (p1.curr==3){
                                                        p1.x=p1.x+SPRITE/2; p1.moving=1;
                                                } else {
                                                        p1.curr=3;
                                                }
					}
				} else if (keystate[SDL_SCANCODE_SPACE]){
					if (p1.choose==0){
						p1.choose=1;
					}
				} else if (keystate[SDL_SCANCODE_L]){
					if (p1.choose==0){
						bombs.push_back(Bomb(p1.x,p1.y,p1.curr,maze,bombidle,bombexp));
			      			p1.choose=2;
					}
				}		
				if (keystate[SDL_SCANCODE_W]){
                                        if (p2.choose==0){
                                                if (p2.curr==0){
                                                        p2.y=p2.y-SPRITE/2; p2.moving=1;
                                                } else {
                                                        p2.curr=0;
                                                }
					}
                                } else if (keystate[SDL_SCANCODE_S]){
                                        if (p2.choose==0){
                                                if (p2.curr==2){
                                                        p2.y=p2.y+SPRITE/2; p2.moving=1;
                                                } else {
                                                        p2.curr=2;
                                                }
                                        }
                                } else if (keystate[SDL_SCANCODE_A]){
                                        if (p2.choose==0){
                                                if (p2.curr==1){
                                                        p2.x=p2.x-SPRITE/2; p2.moving=1;
                                                } else {
                                                        p2.curr=1;
                                                }
                                        }
                                } else if (keystate[SDL_SCANCODE_D]){
                                        if (p2.choose==0){
                                                if (p2.curr==3){
                                                        p2.x=p2.x+SPRITE/2; p2.moving=1;
                                                } else {
                                                        p2.curr=3;
                                                }
                                        }
                                } else if (keystate[SDL_SCANCODE_Q]){
					if (p2.choose==0){
                                        	p2.choose=1;
					}
                                } else if (keystate[SDL_SCANCODE_E]){
					if (p2.choose==0){
						bombs.push_back(Bomb(p2.x,p2.y,p2.curr,maze,bombidle,bombexp));
						p2.choose=2;
					}
				}
				while(SDL_PollEvent(&e)){
					if (e.type==SDL_QUIT){
						quit=true;
					}
				}
				if (CollisionMaze(p1.x,p1.y,SCREEN_WIDTH,SCREEN_HEIGHT,SPRITE,maze,loc)){
					p1.x=store[0];
					p1.y=store[1];
					p1.moving=0;
				}
				if (CollisionMaze(p2.x,p2.y,SCREEN_WIDTH,SCREEN_HEIGHT,SPRITE,maze,loc)){
                                        p2.x=store[2];
                                        p2.y=store[3];
                                        p2.moving=0;
                                }
				vector<pair<int,int>> etemp=move(p1.y,p1.x,p2.y,p2.x,loc);
				for (int i=0;i<bombs.size();++i){
                                        bombs[i].Tick();
                                        bombs[i].RenderBomb(render);
					ehit=bombs[i].check(&p1,&p2,en);
                                }
                                vector<Bomb> temp;
                                for(auto i:bombs){
                                        if (i.spawntimer!=0){
                                                temp.push_back(i);
                                        }
                                }
                                bombs=temp;
				vector<pair<int,int>> tloc;
				vector<Enemy> ten;
				for (int i=0;i<etemp.size();++i){
					if (find(ehit.begin(),ehit.end(),en[i].id)==ehit.end()){
						int curr=finddir(loc[i],etemp[i]);
						en[i].dir=curr;
						en[i].locations=etemp[i];
						en[i].RenderEnemy(etemp[i].first,etemp[i].second,render);
						ten.push_back(en[i]);
						tloc.push_back(etemp[i]);
					}
				}
				if (ehit.size()!=0){
					schedule=10;
				}
				en=ten;
				loc=tloc;
				if (p1.HP<=0 && p2.HP<=0){
					cout << "Mutual Destruction\n";
					quit=true;
				} else if (p1.HP<=0){
					cout << "Player 2 won\n";
					quit=true;
				} else if (p2.HP<=0){
					cout << "Player 1 won\n";
					quit=true;
				}
				p1.RenderPlayer(render,SDL_Rect{p1.x,p1.y,SPRITE,SPRITE});
				p2.RenderPlayer(render,SDL_Rect{p2.x,p2.y,SPRITE,SPRITE});
                        	SDL_RenderPresent(render);
				SDL_RenderClear(render);
				p1.moving=0; p2.moving=0;
				SDL_Delay(1000/12);
			}
		}
	}
	close();
	return 0;
}
