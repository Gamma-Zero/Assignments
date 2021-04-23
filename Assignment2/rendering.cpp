#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
#include "CollisionDetection.h"
#include "maze.h"
#include "AnimationWrappers.h"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
const int SPRITE = 40;
const int FPS = 60;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* render=NULL;
SDL_Texture* p1=NULL;
SDL_Texture* wall=NULL;
vector<vector<bool>> maze;
int ssheeth,ssheetw;

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
		SDL_Texture* gexp=loadTexture(loadBMP("Textures/grass.bmp"));
		if (gexp==NULL){
			printf("Failed\n");
		} else {
			bool quit=false;
			SDL_Event e;
			SDL_RenderClear(render);
			int curr=2; // up left down right
			p1=loadTexture(loadPNG("Textures/p1.png"));
			SDL_QueryTexture(p1,NULL,NULL,&ssheetw,&ssheeth);
			Anim walkanimation=Anim(0,10*ssheeth/21,ssheetw/13,ssheeth/21);
			Anim shootanimation=Anim(0,18*ssheeth/21,ssheetw/13,ssheeth/21);
			wall=loadTexture(loadPNG("Textures/wall.png"));
			int x=40,y=40;
			bool choose=0;
			bool moving=0;
			while(!quit){
				int store[2]={x,y};
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
					if (!choose){
					const Uint8* keystate=SDL_GetKeyboardState(NULL);
					if (keystate[SDL_SCANCODE_UP]){
						if (curr==0){
							y=y-SPRITE/2; moving=1;
                                                } else {
                                                        curr=0;
                                                }
					} else if (keystate[SDL_SCANCODE_DOWN]){
						if (curr==2){
                                                        y=y+SPRITE/2; moving=1;
                                                } else {
                                                        curr=2;
                                                }
					} else if (keystate[SDL_SCANCODE_LEFT]){
						if (curr==1){
                                                        x=x-SPRITE/2; moving=1;
                                                } else {
                                                        curr=1;
                                                }
					} else if (keystate[SDL_SCANCODE_RIGHT]){
						if (curr==3){
                                                        x=x+SPRITE/2; moving=1;
                                                } else {
                                                        curr=3;
                                                }
					} else if (keystate[SDL_SCANCODE_SPACE]){
						choose=1;
					}
					}
					while(SDL_PollEvent(&e)){
						if (e.type==SDL_QUIT){
							quit=true;
						}
					}
					SDL_RenderCopy(render,gexp,NULL,NULL);
					if (CollisionMaze(x,y,SCREEN_WIDTH,SCREEN_HEIGHT,SPRITE,maze)){
						x=store[0];
						y=store[1];
						moving=0;
					}
					for (int i=0;i<maze.size();i++){
						for (int j=0;j<maze.size();j++){
							if (!maze[i][j]){
								SDL_Rect t={i*SPRITE,j*SPRITE,SPRITE,SPRITE};
								SDL_RenderCopy(render,wall,NULL,&t);
							}
						}
					}
					SDL_Rect space={x,y,SPRITE,SPRITE};
					SDL_Texture* texture;
					if (!moving){
						walkanimation.x=0;
					}
					if (!choose){
						walkanimation.y=(curr+8)*ssheeth/21;
						walkanimation.update();
						SDL_RenderCopy(render,p1,&walkanimation.space,&space);
					} else {
						shootanimation.y=(curr+16)*ssheeth/21;
                                                shootanimation.update();
                                                SDL_RenderCopy(render,p1,&shootanimation.space,&space);
					}
                        		SDL_RenderPresent(render);
					moving=0;
					SDL_Delay(1000/15);
			}
		}
	}
	close();
	return 0;
}
