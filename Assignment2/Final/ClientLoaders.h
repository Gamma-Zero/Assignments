#include "ClientInit.h"

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
