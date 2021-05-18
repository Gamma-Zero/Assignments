#include "Bullet.h"
#include "CollisionDetection.h"

struct Bomb{
	int x,y,id;
	int spawntimer=30;	
	SDL_Texture* bombidle;
	SDL_Texture* bombexp;
	int bh,bw; int SPRITE=40;
	Anim bomb;
	vector<vector<bool>> maze;
	vector<pair<int,int>> plumes;
	vector<int> playershurt;
	vector<int> enemieshurt;
	Bomb(int x1, int y1, int dir, vector<vector<bool>> mat, int w, SDL_Texture* bi, SDL_Texture* be){
		id = w;
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
			plumes.push_back({x,y});
			SDL_RenderCopy(render,bombexp,&bomb.space,&space);
			for(int i=0;i<3;++i){
				x+=SPRITE;
				if (CollisionBomb(x,y,1000,1000,SPRITE,maze)){
					break;
				} else {
					if (spawntimer==8) plumes.push_back({x,y});
					space={x,y,SPRITE,SPRITE};
					SDL_RenderCopy(render,bombexp,&bomb.space,&space);
				}
			}
			x=temp[0];
			for(int i=0;i<3;++i){
                                x-=SPRITE;
                                if (CollisionBomb(x,y,1000,1000,SPRITE,maze)){
                                        break;
                                } else {
					if (spawntimer==8) plumes.push_back({x,y});
                                        space={x,y,SPRITE,SPRITE};
                                        SDL_RenderCopy(render,bombexp,&bomb.space,&space);
                                }
                        }
			x=temp[0];
			for(int i=0;i<3;++i){
                                y+=SPRITE;
                                if (CollisionBomb(x,y,1000,1000,SPRITE,maze)){
                                        break;
                                } else {
					if (spawntimer==8) plumes.push_back({x,y});
                                        space={x,y,SPRITE,SPRITE};
                                        SDL_RenderCopy(render,bombexp,&bomb.space,&space);
                                }
                        }
			y=temp[1];
			for(int i=0;i<3;++i){
                                y-=SPRITE;
                                if (CollisionBomb(x,y,1000,1000,SPRITE,maze)){
                                        break;
                                } else 
				{
					if (spawntimer==8) plumes.push_back({x,y});
                                        space={x,y,SPRITE,SPRITE};
                                        SDL_RenderCopy(render,bombexp,&bomb.space,&space);
                                }
                        }
			y=temp[1];
			bomb.x=(bomb.x+bw/8)%bw;
			bomb.update();
		}
	}
	vector<int> check(Player* p1, Player* p2, vector<Enemy> en){
		vector<int> ehit;
		for (auto i:plumes){
			int x=i.first; int y=i.second;
			if ((x/40==p1->x/40) && (y/40==(p1->y+39)/40)){
				if (find(playershurt.begin(),playershurt.end(),1)==playershurt.end()){
					p1->upHP(p1->HP-50);
					playershurt.push_back(1);
				}
			}
			if ((x/40==p2->x/40) && (y/40==(p2->y+39)/40)){
                        	if (find(playershurt.begin(),playershurt.end(),2)==playershurt.end()){
                                	p2->upHP(p2->HP-50);
                                	playershurt.push_back(2);
                        	}
                	}
			for (auto j:en){
				int tx=j.locations.first; int ty=j.locations.second;
				if ((x/40==tx/40) && (y/40==(ty+39)/40)){
                                	if (find(enemieshurt.begin(),enemieshurt.end(),j.id)==enemieshurt.end()){
                                        	j.upHP(j.HP-50);
                                        	enemieshurt.push_back(j.id);
						ehit.push_back(j.id);
                                	}
                        	}
			}
		}
		return ehit;
	}
};

void RenderBox(SDL_Texture* s, SDL_Renderer* render, int x, int y){
	SDL_Rect space={x*40,y*40,40,40};
	SDL_RenderCopy(render,s,NULL,&space);
}
