#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* render=NULL;
SDL_Texture* texture=NULL;

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
                window = SDL_CreateWindow( "Darkest Curse of the Dead Gods", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
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
			int x=0,y=0;
			while(!quit){
				while(SDL_PollEvent(&e)!=0){
					if (e.type==SDL_QUIT){
						quit=true;
					} else if (e.type==SDL_KEYDOWN){
						switch(e.key.keysym.sym){
							case SDLK_UP:
								y=max(y-5,0);
								break;
							case SDLK_DOWN:
								y=min(y+5,430);
                                                                break;
							case SDLK_LEFT:
								x=max(0,x-5);
                                                                break;
							case SDLK_RIGHT:
								x=min(590,x+5);
                                                                break;
							default:
								break;
						}
					}
					SDL_Rect space={x,y,50,50};
					SDL_RenderCopy(render,gexp,NULL,NULL);
                        		SDL_RenderCopy(render,texture,NULL,&space);
                        		SDL_RenderPresent(render);
				}
			}
		}
	}
	close();
	return 0;
}
