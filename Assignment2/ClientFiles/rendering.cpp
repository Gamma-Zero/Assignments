#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <bits/stdc++.h>
#include "parse.h"
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
		if (SDL_Init(SDL_INIT_AUDIO) < 0) {
			exit(1);
		}
		Mix_Init(0);
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
		TTF_Init();
		bgm = Mix_LoadMUS("Audio/bgm.mp3");
		bombsound = Mix_LoadWAV("Audio/bomb.wav");
		bowsound = Mix_LoadWAV("Audio/bow.wav");
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
				SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"linear");
                                SDL_RenderSetLogicalSize(render,1200,1000);
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
                                        maze=vector<vector<bool>>(25,vector<bool>());
                                        for (int i=0;i<625;++i){
                                                if (bufferc[i]=='0'){
                                                        maze[i/25].push_back(0);
                                                } else {
                                                        maze[i/25].push_back(1);
                                                }
                                        }
					SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
					screenSurface = SDL_GetWindowSurface(window);
					font = TTF_OpenFont("Fonts/Raleway-Black.ttf", 20);
				}
			}
		}
	}
	BFS(maze);
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
			valread=read(sock,buffer,1024);
			vector<int> tt=parse((string)buffer);
			vector<pair<int,int>> loc;
			for(int i=11;i<tt.size();i+=4){
					loc.push_back({tt[i],tt[i+1]});
					en.push_back(Enemy(tt[i+3], loadTexture(loadPNG("Textures/enemy.png")), {tt[i],tt[i+1]}));
			}
			Player p1 = Player(loadTexture(loadPNG("Textures/p1.png")), 40,40);
			Player p2 = Player(loadTexture(loadPNG("Textures/p2.png")), 920,920);
			SDL_Texture* bombidle = loadTexture(loadPNG("Textures/bomb.png"));
			SDL_Texture* bombexp = loadTexture(loadPNG("Textures/bombexpnew.png"));
			arrow = loadTexture(loadPNG("Textures/arrow-scaled.png"));
			wall = loadTexture(loadPNG("Textures/wall.png"));
			lb = loadTexture(loadPNG("Textures/lootbox.png"));
			int timer1 = -1; int timer2 = -1; bool bult=0; bool bombt=0;
			Mix_PlayMusic(bgm, 1);
			while (!quit) {
				SDL_RenderCopy(render, gexp, NULL, NULL);
				SDL_Rect out = { 1000,0,200,1000 };
				SDL_RenderFillRect(render, &out);
				for (int i = 0; i < maze.size(); i++) {
					for (int j = 0; j < maze.size(); j++) {
						if (!maze[i][j]) {
							SDL_Rect t = { i * SPRITE,j * SPRITE,SPRITE,SPRITE };
							SDL_RenderCopy(render, wall, NULL, &t);
						}
					}
				}
				frame++;
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
					if (p2.carrow > 0 && p2.choose == 0) {
						Mix_PlayChannel(-1, bowsound, 0);
						p2.carrow--;
						timer2 = 5;
						p2.choose = 1;
						bult=1;
					}
				}
				else if (keystate[SDL_SCANCODE_E]) {
					if (p2.cbomb > 0 && p2.choose == 0) {
						p2.cbomb--;
						bombs.push_back(Bomb(p2.x, p2.y, p2.curr, maze, 2, bombidle, bombexp));
						p2.choose = 2;
						bombt=1;
					}
				}
				int j1 = p1.x / 40, j2 = (p1.y + 39) / 40;
				things[j1][j2].clear();
				j1 = p2.x / 40; j2 = (p2.y + 39) / 40;
				for(auto cc:things[j1][j2])
				if (cc == 1) p2.carrow++;
				else if (cc == 2) p2.HP += 20;
				else if (cc == 3) p2.cbomb++;
				else if (cc == 4) p2.score += 5;
				things[j1][j2].clear();
				p1.HP = min(p1.HP, 100);
				p2.HP = min(p2.HP, 100);
				if (CollisionMaze(p2.x, p2.y, SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE, maze, loc)) {
                                        p2.x = store[2];
                                        p2.y = store[3];
                                        p2.moving = 0;
                                }
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
				string s="";
				string stemp=to_string(p2.x);
				for (int i=0;i<(4-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp;
				stemp=to_string(p2.y);
				for (int i=0;i<(4-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp;
				s+=to_string(p2.curr);
				s+=to_string(p2.moving);
				s+=to_string(p2.choose);
				s+=";"; s+=";";
				if (bombt) s+="1"; else s+="0"; s+=";";
				if (bult) s+="1"; else s+="0"; s+=";";
				stemp=to_string(p2.HP);
                                for (int i=0;i<(3-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp; s+=";";
                                stemp=to_string(p2.score);
                                for (int i=0;i<(3-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp; s+=";";
                                stemp=to_string(p2.cbomb);
                                for (int i=0;i<(2-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp; s+=";";
                                stemp=to_string(p2.carrow);
                                for (int i=0;i<(2-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp;
                                char const *t=s.c_str();
                                send(sock,t,1024,0);
                                valread=read(sock,buffer,1024);
				vector<int> tt=parse((string)buffer);
                                p1.x=tt[0]; p1.y=tt[1]; p1.curr=tt[2]; p1.moving=tt[3]; p1.choose=tt[4];
				p1.carrow=tt[5]; p1.cbomb=tt[6]; p1.score=tt[7]; p1.HP=tt[8];
                                if (tt[9]){
                                                bombs.push_back(Bomb(p1.x, p1.y, p1.curr, maze, 2, bombidle, bombexp));
                                                p1.choose = 2;
                                }
                                if (tt[10]){
                                        Mix_PlayChannel(-1, bowsound, 0);
                                        timer1 = 5;
                                        p1.choose = 1;
                                }
				for(int i=11;i<tt.size();i+=4){
					bool f=0;
					for (int j=0;j<en.size();++j){
						if (en[j].id==tt[i+3]){
							f=1;
							en[j].locations={tt[i],tt[i+1]};
							loc[j]={tt[i],tt[i+1]};
							break;
						}
					}
					for (int j=0;j<tokill.size();++j){
						if (tokill[j].id==tt[i+3]){
							f=1;
							break;
						}
					}
					if (!f){
                                        	loc.push_back({tt[i],tt[i+1]});
                                        	en.push_back(Enemy(tt[i+3], loadTexture(loadPNG("Textures/enemy.png")), {tt[i],tt[i+1]}));
					}
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
				for (int i = 0; i < bombs.size(); ++i) {
					if (bombs[i].spawntimer == 8) {
						Mix_PlayChannel(-1, bombsound, 0);
					}
					if (bombs[i].spawntimer != 0) {
						temp.push_back(bombs[i]);
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
					cout << en[i].id << " " ;
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
						if (pl[it - ehit.begin()] == 1) ++p1.score;
						else ++p2.score;
						int surprise = gift();
						things[etemp[i].first / 40][(etemp[i].second + 39) / 40].push_back(surprise);
						tokill.push_back(en[i]);
					}
				}
				cout << '\n';
				if (ehit.size()) {
					schedule += 20;   //changes
				}
				en = ten;
				loc = tloc;
				ten.clear();
				for (int i = 0; i < tokill.size(); ++i) {
					if (!tokill[i].triggerDeath(render)) {
						ten.push_back(tokill[i]);
					}
				}
				tokill = ten;
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
				for (int i = 0; i < 25; ++i) {
					for (int j = 0; j < 25; ++j) {
						if (things[i][j].size() != 0) {
							RenderBox(lb, render, i, j);
						}
					}
				}
				p1.RenderPlayer(render, SDL_Rect{ p1.x,p1.y,SPRITE,SPRITE });
				p2.RenderPlayer(render, SDL_Rect{ p2.x,p2.y,SPRITE,SPRITE });
				RenderFont(render, font, p1.x, p1.y - 10, p1.HP);
				RenderFont(render, font, p2.x, p2.y - 10, p2.HP);
				for (auto i : en) {
					RenderFont(render, font, i.locations.first, i.locations.second - 10, i.HP);
				}
				RenderData(p1, p2, render, font);
				SDL_RenderPresent(render);
				SDL_RenderClear(render);
				p1.moving = 0; p2.moving = 0;
				SDL_Delay(1000 / 12);
				ehit.clear();
				pl.clear(); bult=0; bombt=0;
			}
		}
	}
	close();
	return 0;
}

