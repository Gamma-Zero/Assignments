#include "Player.h"

struct Enemy {
        SDL_Texture* sprites;
        int id;
        Anim walkanimation;
        Anim deathanimation;
        int ssheeth, ssheetw;
        pair<int, int> locations;
        int dir;
        int HP = 20;
        int SPRITE = 40;
        Enemy(int i, SDL_Texture* s, pair<int, int> loc) {
                sprites = s;
                SDL_QueryTexture(s, NULL, NULL, &ssheetw, &ssheeth);
                walkanimation = Anim(0, 10 * ssheeth / 21, ssheetw / 13, ssheeth / 21);
                deathanimation = Anim(0, 20 * ssheeth / 21, ssheetw / 13, ssheeth / 21);
                locations = loc;
                dir = 9;
                id = i;
        }
        void RenderEnemy(int x1, int y1, SDL_Renderer* render) {
                if (dir==9){
                        walkanimation.x=0;
                } else {
                        walkanimation.x = (walkanimation.x + ssheetw / 13) % (9 * ssheetw / 13);
                }
                walkanimation.y = (dir + 8) * ssheeth / 21;
                walkanimation.update();
                SDL_Rect space = { x1,y1,SPRITE,SPRITE };
                SDL_RenderCopy(render, sprites, &walkanimation.space, &space);
        }
        void upHP(int val) {
                HP = val;
        }
        bool triggerDeath(SDL_Renderer* render){
                if (deathanimation.x==6*ssheetw/13){
                        return true;
                } else {
                        SDL_Rect space={locations.first,locations.second,40,40};
                        SDL_RenderCopy(render, sprites, &deathanimation.space, &space);
                        deathanimation.x += ssheetw/13;
                        deathanimation.update();
                        return false;
                }
        }
};
