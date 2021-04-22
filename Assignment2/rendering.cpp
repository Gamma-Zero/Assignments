#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
#include "CollisionDetection.h"
#include "maze.h"

using namespace std;

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
const int SPRITE = 40;
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* render=NULL;
SDL_Texture* texture=NULL;
SDL_Texture* wall=NULL;
vector<vector<bool>> maze;

enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

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
				render=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
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
	SDL_Texture* temp=SDL_CreateTextureFromSurface(render,input);
	if (temp==NULL){
		printf("Texture Conversion died\n");
	}
	return temp;
}

void close(){
	SDL_DestroyWindow(window);
	window=NULL;
	SDL_DestroyTexture(texture);
        texture = NULL;
	SDL_DestroyRenderer(render);
	render=NULL;
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[]){
	if (!init()){
		printf("Failed\n");
	} else {
		SDL_Texture* gexp=loadTexture(loadBMP("eg.bmp"));
		if (gexp==NULL){
			printf("Failed\n");
		} else {
			bool quit=false;
			SDL_Event e;
			SDL_RenderClear(render);
                        texture=loadTexture(loadPNG("eg.png"));
			wall=loadTexture(loadPNG("wall.png"));
			int x=40,y=40;
			while(!quit){
				while(SDL_PollEvent(&e)!=0){
					int store[2]={x,y};
					if (e.type==SDL_QUIT){
						quit=true;
					} else if (e.type==SDL_KEYDOWN){
						switch(e.key.keysym.sym){
							case SDLK_UP:
								y=y-SPRITE;
								break;
							case SDLK_DOWN:
								y=y+SPRITE;
								break;
							case SDLK_LEFT:
								x=x-SPRITE;
								break;
							case SDLK_RIGHT:
								x=x+SPRITE;
								break;
							default:
								break;
						}
					}
					SDL_RenderCopy(render,gexp,NULL,NULL);
					if (CollisionMaze(x,y,SCREEN_WIDTH,SCREEN_HEIGHT,SPRITE,maze)){
						x=store[0];
						y=store[1];
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
                        		SDL_RenderCopy(render,texture,NULL,&space);
                        		SDL_RenderPresent(render);
				}
			}
		}
	}
	close();
	return 0;
}
