#include "Enemy.h"

struct bullet{
	SDL_Texture* sprite;
	int movedir, dis, id, status, curx, cury;
	int sh,sw;
	bullet(SDL_Texture* s, int direction, int x, int y, int pl_id)
	{
		sprite=s;
		SDL_QueryTexture(s, NULL, NULL, &sw, &sh);
		movedir = direction;
		x /= 40;
		y += 39;
		y /= 40;
		assert(cell[x][y]);
		curx = x;
		cury = y;
		status = 1;
		id = pl_id;
		dis = 0;
	}
	int mvblt(vector<Enemy>en, int x1, int y1, int x2, int y2, int& health1, int& health2)
	{
		x1 /= 40;
		x2 /= 40;
		y1 += 39;
		y2 += 39;
		y1 /= 40;
		y2 /= 40;
		assert(cell[x1][y1]);
		assert(cell[x2][y2]);
		dis++;
		if (dis > 5)
		{
			status = 0;
			return -1;
		}
		if (movedir == 0) cury--;
		else if (movedir == 1) curx--;
		else if (movedir == 2) cury++;
		else curx++;
		if (curx < 0 || curx>24 || cury < 0 || cury>24 || !cell[curx][cury])
		{
			status = 0;
			return -1;
		}
		if (curx == x1 && cury == y1)
		{
			health1 -= 20;
			status = 0;
			return -1;
		}
		else if (curx == x2 && cury == y2)
		{
			health2 -= 20;
			status = 0;
			return -1;
		}
		for (int i = 0; i < en.size(); ++i)
		{
			int enx = en[i].locations.first / 40, eny = en[i].locations.second / 40;
			if (!cell[enx][eny]) ++eny;
			if (status == 1 && enx == curx && eny == cury)
			{
				status = 0;
				return en[i].id;
			}
			if (status == 1 && abs(enx - curx) + abs(eny - cury) == 1)
			{
				if (enx == curx + 1 && movedir == 3)
				{
					status = 0;
					return en[i].id;
				}
				else if (enx == curx - 1 && movedir == 1)
				{
					status = 0;
					return en[i].id;
				}
				else if (eny == cury - 1 && movedir==0)
				{
					status = 0;
					return en[i].id;
				}
				else if (eny == cury + 1 && movedir == 2)
				{
					status = 0;
					return en[i].id;
				}
			}
		}
		return -1;
	}
	void RenderBullet(SDL_Renderer* render){
		SDL_Rect space={curx*40,cury*40,40,40};
		SDL_Rect renderspace={movedir*sw/4,0,sw/4,sh};
		SDL_RenderCopy(render,sprite,&renderspace,&space);
	}
};
