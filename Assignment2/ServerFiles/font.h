#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* loadTexture(SDL_Surface* input){
        Uint32 colorkey=SDL_MapRGB(input->format,0,0,0);
        SDL_SetColorKey(input,SDL_TRUE,colorkey);
        SDL_Texture* temp=SDL_CreateTextureFromSurface(render,input);
        if (temp==NULL){
                printf("Texture Conversion died\n");
        }
        return temp;
}

void RenderFont(SDL_Renderer* render, TTF_Font *font, int x, int y, int text){
	string t=to_string(text);
	SDL_Color fcolor={173,216,230};
	SDL_Color bcolor={0,0,0};
	SDL_Surface* temp=TTF_RenderText_Shaded(font,t.c_str(),fcolor,bcolor);
	SDL_Texture* tx=loadTexture(temp);
	SDL_Rect space={x,y,20,20};
	SDL_RenderCopy(render,tx,NULL,&space);
}
