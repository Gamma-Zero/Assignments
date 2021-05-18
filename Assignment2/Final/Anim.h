#include <SDL2/SDL.h>
#include "spawnnew.h"
#include "reward.h"

struct Anim {
        SDL_Rect space;
        int x, y, w, h;
        Anim() {
                space = { 0,0,0,0 };
                x = 0; y = 0; w = 0; h = 0;
        }
        Anim(int x1, int y1, int w1, int h1) {
                space = { x1,y1,w1,h1 };
                x = x1;
                y = y1;
                w = w1;
                h = h1;
        }
        void update() {
                space = { x,y,w,h };
        }
};

