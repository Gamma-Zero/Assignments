#include "Init.h"

int finddir(pair<int, int> s, pair<int, int> e) {
        if (e.first < s.first) {
                return 1;
        }
        else if (e.first > s.first) {
                return 3;
        }
        else if (e.second < s.second) {
                return 0;
        }
        else if (e.second > s.second) {
                return 2;
        } else {
                return 9;
        }
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

void printscr(string idle, int x, int y){
        SDL_Color fcolor={173,216,230};
        SDL_Color bcolor={0,0,0};
        SDL_Surface* t=TTF_RenderText_Shaded(font,idle.c_str(),fcolor,bcolor);
        SDL_Texture* tx=loadTexture(t);
        SDL_Rect space={x,y,10*idle.size(),20};
        SDL_RenderCopy(render,tx,NULL,&space);
}

SDL_Surface* loadBMP(string path) {
        SDL_Surface* temp = SDL_LoadBMP(path.c_str());
        if (temp == NULL) {
                printf("Load failed\n");
        }
        return temp;
}

SDL_Surface* loadPNG(string path) {
        SDL_Surface* opt = NULL;
        SDL_Surface* temp = IMG_Load(path.c_str());
        if (temp == NULL) {
                printf("PNG Load failed\n");
        }
        else {
                opt = SDL_ConvertSurface(temp, screenSurface->format, 0);
                if (opt == NULL) {
                        printf("Could not optimize\n");
                }
                SDL_FreeSurface(temp);
        }
        return opt;
}

void close() {
        SDL_DestroyWindow(window);
        window = NULL;
        SDL_DestroyRenderer(render);
        render = NULL;
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
}
