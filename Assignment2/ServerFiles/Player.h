#include "Anim.h"

struct Player {
	SDL_Texture* sprites;
	Anim walkanimation;
	Anim shootanimation;
	Anim bombanimation;
	Anim deathanimation;
	int ssheeth, ssheetw;
	int choose;
	int rest=0;
	bool moving;
	int x, y, score=0;
	int curr;
	int HP = 100;
	int carrow = 10, cbomb = 5;
	Player(SDL_Texture* s, int x1, int y1) {
		sprites = s;
		SDL_QueryTexture(s, NULL, NULL, &ssheetw, &ssheeth);
		walkanimation.y = 10 * ssheeth / 21;
		walkanimation.w = ssheetw / 13;
		walkanimation.h = ssheeth / 21;
		shootanimation.y = 18 * ssheeth / 21;
		shootanimation.w = ssheetw / 13;
		shootanimation.h = ssheeth / 21;
		bombanimation.y = 2 * ssheetw / 13;
		bombanimation.w = ssheetw / 13;
		bombanimation.h = ssheeth / 21;
		deathanimation.y = 20 * ssheeth / 21;
                deathanimation.w = ssheetw / 13;
                deathanimation.h = ssheeth / 21;
		x = x1;
		y = y1;
		choose = 0;
		moving = 0;
		curr = 2;
	}
	void RenderPlayer(SDL_Renderer* render, SDL_Rect space) {
		if (!moving) {
			walkanimation.x = 0;
		}
		if (choose == 0) {
			walkanimation.y = (curr + 8) * ssheeth / 21;
			walkanimation.update();
			SDL_RenderCopy(render, sprites, &walkanimation.space, &space);
		}
		else if (choose == 1) {
			shootanimation.y = (curr + 16) * ssheeth / 21;
			shootanimation.update();
			SDL_RenderCopy(render, sprites, &shootanimation.space, &space);
		}
		else if (choose == 2) {
			bombanimation.y = (curr)*ssheeth / 21;
			bombanimation.update();
			SDL_RenderCopy(render, sprites, &bombanimation.space, &space);
		}

	}
	void passiveAnimate() {
		if (choose == 0) {
			walkanimation.x = (walkanimation.x + ssheetw / 13) % (9 * ssheetw / 13);
			shootanimation.x = 0;
			bombanimation.x = 0;
		}
		else if (choose == 1) {
			shootanimation.x = shootanimation.x + ssheetw / 13;
			walkanimation.x = 0;
			if (shootanimation.x == ssheetw) {
				choose = 0;
				shootanimation.x = 0;
			}
		}
		else if (choose == 2) {
			bombanimation.x = bombanimation.x + ssheetw / 13;
			walkanimation.x = 0;
			if (bombanimation.x == 7 * ssheetw / 13) {
				choose = 0;
				bombanimation.x = 0;
			}
		}
	}
	void upHP(int val) {
		HP = val;
	}
	bool triggerDeath(SDL_Renderer* render){
                if (deathanimation.x==6*ssheetw/13){
                        return true;
                } else {
                        SDL_Rect space={x,y,40,40};
                        SDL_RenderCopy(render, sprites, &deathanimation.space, &space);
                        deathanimation.x += ssheetw/13;
			deathanimation.update();
                        return false;
                }
        }
};
