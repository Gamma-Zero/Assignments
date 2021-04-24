#include "AnimationWrappers.h"
#include "CollisionDetection.h"

struct Bomb{
	int x,y;
	int spawntimer=40;	
	SDL_Texture* bombidle;
	SDL_Texture* bombexp;
	int bh,bw; int SPRITE=40;
	Anim bomb;
	vector<vector<bool>> maze;
	Bomb(int x1, int y1, int dir, vector<vector<bool>> mat, SDL_Texture* bi, SDL_Texture* be){
		y=((y1+SPRITE-1)/SPRITE)*SPRITE;
		if (dir==3){
			x=((x1+SPRITE-1)/SPRITE)*SPRITE;
		} else {
			x=(x1/SPRITE)*SPRITE;
		}
		bombidle=bi;
		bombexp=be;
		SDL_QueryTexture(be,NULL,NULL,&bw,&bh);
		bomb.h=bh;
		bomb.w=bw/8;
		bomb.update();
		maze=mat;
	}
	void Tick(){
		spawntimer--;
	}
	void RenderBomb(SDL_Renderer* render){
		if (spawntimer>8){
			SDL_Rect space={x,y,SPRITE,SPRITE};
			SDL_RenderCopy(render,bombidle,NULL,&space);
		} else {
			int temp[2]={x,y};
			SDL_Rect space={x,y,SPRITE,SPRITE};
			SDL_RenderCopy(render,bombexp,&bomb.space,&space);
			for(int i=0;i<5;++i){
				x+=SPRITE;
				if (CollisionBomb(x,y,1000,1000,SPRITE,maze)){
					break;
				} else {
					space={x,y,SPRITE,SPRITE};
					SDL_RenderCopy(render,bombexp,&bomb.space,&space);
				}
			}
			x=temp[0];
			for(int i=0;i<5;++i){
                                x-=SPRITE;
                                if (CollisionBomb(x,y,1000,1000,SPRITE,maze)){
                                        break;
                                } else {
                                        space={x,y,SPRITE,SPRITE};
                                        SDL_RenderCopy(render,bombexp,&bomb.space,&space);
                                }
                        }
			x=temp[0];
			for(int i=0;i<5;++i){
                                y+=SPRITE;
                                if (CollisionBomb(x,y,1000,1000,SPRITE,maze)){
                                        break;
                                } else {
                                        space={x,y,SPRITE,SPRITE};
                                        SDL_RenderCopy(render,bombexp,&bomb.space,&space);
                                }
                        }
			y=temp[1];
			for(int i=0;i<5;++i){
                                y-=SPRITE;
                                if (CollisionBomb(x,y,1000,1000,SPRITE,maze)){
                                        break;
                                } else {
                                        space={x,y,SPRITE,SPRITE};
                                        SDL_RenderCopy(render,bombexp,&bomb.space,&space);
                                }
                        }
			y=temp[1];
			bomb.x=(bomb.x+bw/8)%bw;
			bomb.update();
		}
	}
};
