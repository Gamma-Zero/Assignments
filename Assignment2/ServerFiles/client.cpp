#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
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
		if (SDL_Init(SDL_INIT_AUDIO) < 0){
			exit(1);
		}
		Mix_Init(0);
		Mix_OpenAudio(22050,AUDIO_S16SYS,2,640);
		TTF_Init();
		bgm=Mix_LoadMUS("Audio/bgm.mp3");
		window = SDL_CreateWindow("Darkest Curse of the Dead Cells", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
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
					if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    					{
        					printf("\n Socket creation error \n");
        					return -1;
    					}
    					serv_addr.sin_family = AF_INET;
    					serv_addr.sin_port = htons(PORT);
    					if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    					{
        					printf("\nInvalid address/ Address not supported \n");
        					return -1;
    					}
    					if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    					{
        					printf("\nConnection Failed \n");
        					return -1;
    					}
					valreadc=read(sock,bufferc,1024);
					for (int i=0;i<625;++i){
						if (buffer[i]=='0'){
							maze[i/25][i%25]=0;
						} else {
							maze[i/25][i%25]=1;
						}
					}
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
			vector<pair<int, int>> loc = randspawn(40, 40, 920, 920);
			for (auto i : loc) {
				eid++;
				en.push_back(Enemy(eid, loadTexture(loadPNG("Textures/enemy.png")), i));
			}
			Player p1 = Player(loadTexture(loadPNG("Textures/p1.png")), 40, 40);
			Player p2 = Player(loadTexture(loadPNG("Textures/p2.png")), 920, 920);
			SDL_Texture* bombidle = loadTexture(loadPNG("Textures/bomb.png"));
			SDL_Texture* bombexp = loadTexture(loadPNG("Textures/bombexpnew.png"));
			arrow = loadTexture(loadPNG("Textures/arrow-scaled.png"));
			wall = loadTexture(loadPNG("Textures/wall.png"));
			int timer1=-1; int timer2=-1;
			Mix_PlayMusic(bgm,1);
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
					if (schedule % 20 == 0) {
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
					int k1 = i.locations.first / 40, k2 =( i.locations.second + 39)/ 40;
					if (abs(k1-g1)+abs(k2-h1)==1) danger1 = true;
					if (abs(k1-g2)+abs(k2-h2)==1) danger2 = true;
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
							if(!CollisionP1P2(p1.x+20, p1.y, p2.x, p2.y))
								p1.x = p1.x + SPRITE / 2; p1.moving = 1;
						}
						else {
							p1.curr = 3;
						}
					}
				}
				else if (keystate[SDL_SCANCODE_SPACE]) {
					if (p1.choose == 0) {
						timer1=5;
						p1.choose = 1;
					}
				}
				else if (keystate[SDL_SCANCODE_L]) {
					if (p1.choose == 0) {
						bombs.push_back(Bomb(p1.x, p1.y, p1.curr, maze, bombidle, bombexp));
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
					if (p2.choose == 0) {
						timer2=5;
						p2.choose = 1;
					}
				}
				else if (keystate[SDL_SCANCODE_E]) {
					if (p2.choose == 0) {
						bombs.push_back(Bomb(p2.x, p2.y, p2.curr, maze, bombidle, bombexp));
						p2.choose = 2;
					}
				}
				while (SDL_PollEvent(&e)) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}
				if (timer1>0){
                                                timer1--;
                                        } else if (timer1==0){
                                                bul.push_back(bullet(arrow, p1.curr, p1.x, p1.y, 1));
                                                timer1--;
                                        }
				if (timer2>0){
                                                timer2--;
                                        } else if (timer2==0){
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
						ehit.push_back(j);
				}
				for (int i=0;i<etemp.size();++i){
					en[i].locations=etemp[i];
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
					if(ehitt!=-1) ehit.push_back(ehitt);
					if (bul[i].status == 1) bul1.push_back(bul[i]);
				}
				bul = bul1;
				for (auto i:bul){
					i.RenderBullet(render);
				}
				vector<pair<int, int>> tloc;
				vector<Enemy> ten;
				for (int i = 0; i < etemp.size(); ++i) {
					if (find(ehit.begin(), ehit.end(), en[i].id) == ehit.end()) {
						int curr = finddir(loc[i], etemp[i]);
						en[i].dir = curr;
						en[i].locations = etemp[i];
						en[i].RenderEnemy(etemp[i].first, etemp[i].second, render);
						ten.push_back(en[i]);
						tloc.push_back(etemp[i]);
					} else {
						tokill.push_back(en[i]);
					}
				}
				if (ehit.size()) {
					schedule += 20;
				}
				en = ten;
				loc = tloc;
				ten.clear();
				for (int i = 0;i<tokill.size();++i){
					if (!tokill[i].triggerDeath(render)){
						ten.push_back(tokill[i]);
					}
				}
				tokill=ten;
				if (p1.HP <= 0 && p2.HP <= 0) {
					cout << "Mutual Destruction\n";
					quit = true;
				}
				else if (p1.HP <= 0) {
					cout << "Player 2 won\n";
					quit = true;
				}
				else if (p2.HP <= 0) {
					cout << "Player 1 won\n";
					quit = true;
				}
				p1.RenderPlayer(render, SDL_Rect{ p1.x,p1.y,SPRITE,SPRITE });
				p2.RenderPlayer(render, SDL_Rect{ p2.x,p2.y,SPRITE,SPRITE });
				RenderFont(render, font, p1.x, p1.y - 10, max(p1.HP,0));
				RenderFont(render, font, p2.x, p2.y - 10, max(p2.HP,0));
				for (auto i : en) {
					RenderFont(render, font, i.locations.first, i.locations.second - 10, i.HP);
				}
				SDL_RenderPresent(render);
				SDL_RenderClear(render);
				p1.moving = 0; p2.moving = 0;
				SDL_Delay(1000 / 12);
				ehit.clear();
			}
		}
	}
	close();
	return 0;
}
