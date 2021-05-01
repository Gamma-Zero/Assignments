#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <bits/stdc++.h>
#include "projectiles.h"
#include "Global.h"
#include "font.h"

using namespace std;

bool init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL died: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		TTF_Init();
		window = SDL_CreateWindow("Darkest Curse of the Dead Cells", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if (window == NULL)
		{
			printf("Window died: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) & imgFlags)) {
				printf("SDL_Image died\n");
				success = false;
			}
			else {
				render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (render == NULL) {
					printf("Renderer died\n");
					success = false;
				}
				else {
					maze = MazeGenerate();
					SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);
					screenSurface = SDL_GetWindowSurface(window);
					font = TTF_OpenFont("Fonts/Raleway-Black.ttf", 20);
				}
			}
		}
	}
	return success;
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

int main(int argc, char* args[]) {
	if (!init()) {
		printf("Failed\n");
	}
	else {
		SDL_Texture* gexp = loadTexture(loadPNG("Textures/grass.png"));
		if (gexp == NULL) {
			printf("Failed\n");
		}
		else {
			bool quit = false;
			SDL_Event e;
			SDL_RenderClear(render);
			vector<pair<int, int>> loc = randspawn(200,200, 680, 680);
			for (auto i : loc) {
				eid++;
				en.push_back(Enemy(eid, loadTexture(loadPNG("Textures/enemy.png")), i));
			}
			Player p1 = Player(loadTexture(loadPNG("Textures/p1.png")), 200, 200);
			Player p2 = Player(loadTexture(loadPNG("Textures/p2.png")), 680, 680);
			SDL_Texture* bombidle = loadTexture(loadPNG("Textures/bomb.png"));
			SDL_Texture* bombexp = loadTexture(loadPNG("Textures/bombexpnew.png"));
			arrow = loadTexture(loadPNG("Textures/arrow-scaled.png"));
			wall = loadTexture(loadPNG("Textures/wall.png"));
			int timer1 = -1; int timer2 = -1;
			while (!quit) {
				SDL_RenderCopy(render, gexp, NULL, NULL);
				for (int i = 0; i < maze.size(); i++) {
					for (int j = 0; j < maze.size(); j++) {
						if (!maze[i][j]) {
							SDL_Rect t = { i * SPRITE,j * SPRITE,SPRITE,SPRITE };
							SDL_RenderCopy(render, wall, NULL, &t);
						}
					}
				}
				frame++;
				if (schedule != -1) {
					schedule--;
					if (schedule % 100 == 0) {
						eid++;
						pair<int, int> temp = pspawn(p1.x, p1.y, p2.x, p2.y, loc);
						en.push_back(Enemy(eid, loadTexture(loadPNG("Textures/enemy.png")), temp));
						loc.push_back(temp);
					}
				}
				if (frame == 360) {
					frame = 0;
					eid++;
					pair<int, int> temp = pspawn(p1.x, p1.y, p2.x, p2.y, loc);
					en.push_back(Enemy(eid, loadTexture(loadPNG("Textures/enemy.png")), temp));
					loc.push_back(temp);
				}
				int store[4] = { p1.x,p1.y,p2.x,p2.y };
				p1.passiveAnimate();
				p2.passiveAnimate();
				SDL_PumpEvents();
				const Uint8* keystate = SDL_GetKeyboardState(NULL);
				bool danger1 = false, danger2 = false;
				int g1 = p1.x / 40, g2 = p2.x / 40, h1 = (p1.y + 39) / 40, h2 = (p2.y + 39) / 40;
				for (auto i : en)
				{
					int k1 = i.locations.first / 40, k2 = (i.locations.second + 39) / 40;
					if (abs(k1 - g1) + abs(k2 - h1) == 1) danger1 = true;
					if (abs(k1 - g2) + abs(k2 - h2) == 1) danger2 = true;
				}
				if (danger1)
				{
					if (p1.rest == 0)
					{
						p1.HP -= 10;
						p1.rest = 21;
					}
				}
				if (danger2)
				{
					if (p2.rest == 0)
					{
						p2.HP -= 10;
						p2.rest = 21;
					}
				}
				if (p1.rest > 0) --p1.rest;
				if (p2.rest > 0) --p2.rest;
				if (keystate[SDL_SCANCODE_UP]) {
					if (p1.choose == 0) {
						if (p1.curr == 0) {
							if (!CollisionP1P2(p1.x, p1.y - 20, p2.x, p2.y))
								p1.y = p1.y - SPRITE / 2; p1.moving = 1;
						}
						else {
							p1.curr = 0;
						}
					}
				}
				else if (keystate[SDL_SCANCODE_DOWN]) {
					if (p1.choose == 0) {
						if (p1.curr == 2) {
							if (!CollisionP1P2(p1.x, p1.y + 20, p2.x, p2.y))
								p1.y = p1.y + SPRITE / 2; p1.moving = 1;
						}
						else {
							p1.curr = 2;
						}
					}
				}
				else if (keystate[SDL_SCANCODE_LEFT]) {
					if (p1.choose == 0) {
						if (p1.curr == 1) {
							if (!CollisionP1P2(p1.x - 20, p1.y, p2.x, p2.y))
								p1.x = p1.x - SPRITE / 2; p1.moving = 1;
						}
						else {
							p1.curr = 1;
						}
					}
				}
				else if (keystate[SDL_SCANCODE_RIGHT]) {
					if (p1.choose == 0) {
						if (p1.curr == 3) {
							if (!CollisionP1P2(p1.x + 20, p1.y, p2.x, p2.y))
								p1.x = p1.x + SPRITE / 2; p1.moving = 1;
						}
						else {
							p1.curr = 3;
						}
					}
				}
				else if (keystate[SDL_SCANCODE_SPACE]) {
					if (p1.carrow>0 && p1.choose == 0) {
						p1.carrow--;
						timer1 = 5;
						p1.choose = 1;
					}
				}
				else if (keystate[SDL_SCANCODE_L]) {
					if (p1.cbomb>0 && p1.choose == 0) {
						p1.cbomb--;
						bombs.push_back(Bomb(p1.x, p1.y, p1.curr, maze, 1, bombidle, bombexp));
						p1.choose = 2;
					}
				}
				if (keystate[SDL_SCANCODE_W]) {
					if (p2.choose == 0) {
						if (p2.curr == 0) {
							if (!CollisionP1P2(p1.x, p1.y, p2.x, p2.y - 20))
								p2.y = p2.y - SPRITE / 2; p2.moving = 1;
						}
						else {
							p2.curr = 0;
						}
					}
				}
				else if (keystate[SDL_SCANCODE_S]) {
					if (p2.choose == 0) {
						if (p2.curr == 2) {
							if (!CollisionP1P2(p1.x, p1.y, p2.x, p2.y + 20))
								p2.y = p2.y + SPRITE / 2; p2.moving = 1;
						}
						else {
							p2.curr = 2;
						}
					}
				}
				else if (keystate[SDL_SCANCODE_A]) {
					if (p2.choose == 0) {
						if (p2.curr == 1) {
							if (!CollisionP1P2(p1.x, p1.y, p2.x - 20, p2.y))
								p2.x = p2.x - SPRITE / 2; p2.moving = 1;
						}
						else {
							p2.curr = 1;
						}
					}
				}
				else if (keystate[SDL_SCANCODE_D]) {
					if (p2.choose == 0) {
						if (p2.curr == 3) {
							if (!CollisionP1P2(p1.x, p1.y, p2.x + 20, p2.y))
								p2.x = p2.x + SPRITE / 2; p2.moving = 1;
						}
						else {
							p2.curr = 3;
						}
					}
				}
				else if (keystate[SDL_SCANCODE_Q]) {
					if (p2.carrow>0 && p2.choose == 0) {
						p2.carrow--;
						timer2 = 5;
						p2.choose = 1;
					}
				}
				else if (keystate[SDL_SCANCODE_E]) {
					if (p2.cbomb>0 && p2.choose == 0) {
						p2.cbomb--;
						bombs.push_back(Bomb(p2.x, p2.y, p2.curr, maze,2, bombidle, bombexp));
						p2.choose = 2;
					}
				}


				int j1 = p1.x / 40, j2 = (p1.y + 39) / 40;
				if (things[j1][j2] == 1) p1.carrow++;
				else if (things[j1][j2] == 2) p1.HP += 20;
				else if (things[j1][j2] == 3) p1.cbomb++;
				else if(things[j1][j2]==4) p1.score+=5;
				//cout <<1<<' '<< p1.score << ' ' << p1.carrow << ' ' << p1.cbomb << ' ' << p1.HP << '\n';
				things[j1][j2] = 0;
				j1 = p2.x / 40; j2 = (p2.y + 39) / 40;
				if (things[j1][j2] == 1) p2.carrow++;
				else if (things[j1][j2] == 2) p2.HP += 20;
				else if (things[j1][j2] == 3) p2.cbomb++;
				else if (things[j1][j2] == 4) p2.score+=5;
				things[j1][j2] = 0;
				p1.HP = min(p1.HP, 100);
				p2.HP = min(p2.HP, 100);
				//cout << 2<<' '<<p2.score << ' ' << p2.carrow << ' ' << p2.cbomb << ' ' << p2.HP << '\n';


				while (SDL_PollEvent(&e)) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}
				if (timer1 > 0) {
					timer1--;
				}
				else if (timer1 == 0) {
					bul.push_back(bullet(arrow, p1.curr, p1.x, p1.y, 1));
					timer1--;
				}
				if (timer2 > 0) {
					timer2--;
				}
				else if (timer2 == 0) {
					bul.push_back(bullet(arrow, p2.curr, p2.x, p2.y, 2));
					timer2--;
				}
				if (CollisionMaze(p1.x, p1.y, SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE, maze, loc)) {
					p1.x = store[0];
					p1.y = store[1];
					p1.moving = 0;
				}
				if (CollisionMaze(p2.x, p2.y, SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE, maze, loc)) {
					p2.x = store[2];
					p2.y = store[3];
					p2.moving = 0;
				}
				vector<pair<int, int>> etemp = move(p1.y, p1.x, p2.y, p2.x, loc);
				for (int i = 0; i < bombs.size(); ++i) {
					bombs[i].Tick();
					bombs[i].RenderBomb(render);
					vector<int>ehitt = bombs[i].check(&p1, &p2, en);
					for (auto j : ehitt)
					{
						if (find(ehit.begin(), ehit.end(), j) == ehit.end())
						{
							ehit.push_back(j);
							pl.push_back(bombs[i].id);
						}
					}
				}
				for (int i = 0; i < etemp.size(); ++i) {
					en[i].locations = etemp[i];
				}
				vector<Bomb> temp;
				for (auto i : bombs) {
					if (i.spawntimer != 0) {
						temp.push_back(i);
					}
				}
				bombs = temp;
				vector<bullet>bul1;
				for (int i = 0; i < bul.size(); ++i)
				{
					int ehitt = bul[i].mvblt(en, p1.x, p1.y, p2.x, p2.y, p1.HP, p2.HP);
					if (ehitt != -1 && find(ehit.begin(), ehit.end(), ehitt) == ehit.end())
					{
						ehit.push_back(ehitt);
						pl.push_back(bul[i].id);
					}
					if (bul[i].status == 1) bul1.push_back(bul[i]);
				}
				bul = bul1;
				for (auto i : bul) {
					i.RenderBullet(render);
				}
				vector<pair<int, int>> tloc;
				vector<Enemy> ten;
				for (int i = 0; i < etemp.size(); ++i) {
					auto it = find(ehit.begin(), ehit.end(), en[i].id);
					if (it == ehit.end()) {
						int curr = finddir(loc[i], etemp[i]);
						en[i].dir = curr;
						en[i].locations = etemp[i];
						en[i].RenderEnemy(etemp[i].first, etemp[i].second, render);
						ten.push_back(en[i]);
						tloc.push_back(etemp[i]);
					}
					else
					{
						if (pl[it-ehit.begin()] == 1) ++p1.score;
						else ++p2.score;
						int surprise=gift();
						things[etemp[i].first / 40][(etemp[i].second + 39) / 40] = surprise;
					}
				}
				if (ehit.size()) {
					schedule += ehit.size() * 100;   //changes
				}
				en = ten;
				loc = tloc;
				p1.HP = max(p1.HP, 0);
				p2.HP = max(p2.HP, 0);
				if (p1.HP > 0 && p2.HP <= 0)
				{
					cout << "Player 1 Wins!!";
					quit = true;
				}
				else if (p2.HP > 0 && p1.HP <= 0)
				{
					cout << "Player 2 Wins!!";
					quit = true;
				}
				else if (p1.HP <= 0 && p2.HP <= 0)
				{
					if (p1.score > p2.score) cout << "Player 1 Wins";
					else if (p2.score > p1.score) cout << "Player 2 Wins";
					else cout << "Mutual Destruction";
					quit = true;
				}
				p1.RenderPlayer(render, SDL_Rect{ p1.x,p1.y,SPRITE,SPRITE });
				p2.RenderPlayer(render, SDL_Rect{ p2.x,p2.y,SPRITE,SPRITE });
				RenderFont(render, font, p1.x, p1.y - 10, p1.HP);
				RenderFont(render, font, p2.x, p2.y - 10, p2.HP);
				for (auto i : en) {
					RenderFont(render, font, i.locations.first, i.locations.second - 10, i.HP);
				}
				SDL_RenderPresent(render);
				SDL_RenderClear(render);
				p1.moving = 0; p2.moving = 0;
				SDL_Delay(1000 / 12);
				ehit.clear();
				pl.clear();
			}
		}
	}
	close();
	return 0;
}
