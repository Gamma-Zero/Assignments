#include "ServerLoaders.h"

int main(int argc, char* args[]) {
	if (!init(args[1],args[2])) {
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
			vector<pair<int, int>> loc = randspawn(40,40, 920,920);
			for (auto i : loc) {
				eid++;
				en.push_back(Enemy(eid, loadTexture(loadPNG("Textures/enemy.png")), i));
			}
			string s="00400040000;";
			for (auto i:en){
				string temp=to_string(i.locations.first);
				for (int i=0;i<(4-temp.size());++i){
					s+="0";
				}
				s+=temp;
				temp=to_string(i.locations.second);
                                for (int i=0;i<(4-temp.size());++i){
                                        s+="0";
                                }
                                s+=temp;
				s+=to_string(i.dir);
				temp=to_string(i.id);
				for (int i=0;i<(3-temp.size());++i){
                                        s+="0";
                                }
				s+=temp;
			}
			s+=";0;0;1000000510";
			const char *t=s.c_str();
			send(new_socket,t,1024,0);
			Player p1 = Player(loadTexture(loadPNG("Textures/p1.png")), 40,40);
			Player p2 = Player(loadTexture(loadPNG("Textures/p2.png")), 920,920);
			SDL_Texture* bombidle = loadTexture(loadPNG("Textures/bomb.png"));
			SDL_Texture* bombexp = loadTexture(loadPNG("Textures/bombexpnew.png"));
			arrow = loadTexture(loadPNG("Textures/arrow-scaled.png"));
			wall = loadTexture(loadPNG("Textures/wall.png"));
			lb = loadTexture(loadPNG("Textures/lootbox.png"));
			int timer1 = -1; int timer2 = -1;
			Mix_PlayMusic(bgm, 1);
			bool bombt=0; bool bult=0;
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
					if (p1.carrow > 0 && p1.choose == 0) {
						Mix_PlayChannel(-1, bowsound, 0);
						p1.carrow--;
						timer1 = 5;
						p1.choose = 1;
						bult=1;
					}
				}
				else if (keystate[SDL_SCANCODE_L]) {
					if (p1.cbomb > 0 && p1.choose == 0) {
						p1.cbomb--;
						bombs.push_back(Bomb(p1.x, p1.y, p1.curr, maze, 1, bombidle, bombexp));
						p1.choose = 2;
						bombt=1;
					}
				}
				int j1 = p1.x / 40, j2 = (p1.y + 39) / 40;
				for(auto cc:things[j1][j2])
				if (cc == 1) p1.carrow++;
				else if (cc == 2) p1.HP += 20;
				else if (cc == 3) p1.cbomb++;
				else if (cc == 4) p1.score += 5;
				things[j1][j2].clear();
				j1 = p2.x / 40; j2 = (p2.y + 39) / 40;
                                things[j1][j2].clear();
				p1.HP = min(p1.HP, 100);
				p2.HP = min(p2.HP, 100);


				while (SDL_PollEvent(&e)) {
					if (e.type == SDL_QUIT) {
						quit = true;
					}
				}
				if (CollisionMaze(p1.x, p1.y, SCREEN_WIDTH, SCREEN_HEIGHT, SPRITE, maze, loc)) {
                                        p1.x = store[0];
                                        p1.y = store[1];
                                        p1.moving = 0;
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
				valread=read(new_socket,buffer,1024);
				vector<int> tt=parse((string)buffer);
				p2.x=tt[0]; p2.y=tt[1]; p2.curr=tt[2]; p2.moving=tt[3]; p2.choose=tt[4];
				p2.carrow=tt[5]; p2.cbomb=tt[6]; p2.score=tt[7]; p2.HP=tt[8];
				if (tt[9]){
                                                bombs.push_back(Bomb(p2.x, p2.y, p2.curr, maze, 2, bombidle, bombexp));
                                                p2.choose = 2;
				}
				if (tt[10]){
					Mix_PlayChannel(-1, bowsound, 0);
                                        timer2 = 5;
                                        p2.choose = 1;
				}
				string s="";
                                string stemp=to_string(p1.x);
                                for (int i=0;i<(4-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp;
                                stemp=to_string(p1.y);
                                for (int i=0;i<(4-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp;
                                s+=to_string(p1.curr);
                                s+=to_string(p1.moving);
                                s+=to_string(p1.choose);
                                s+=";"; 
								for (int i = 0; i < (int)en.size(); ++i)
								{
									int v1= en[i].locations.first, v2=en[i].locations.second, v3=en[i].dir, v4=en[i].id ;
									string ss1 = to_string(v1), ss2 = to_string(v2), ss3 = to_string(v3), ss4 = to_string(v4);
									while ((int)ss1.size() < 4) ss1 = "0" + ss1;
									while ((int)ss2.size() < 4) ss2 = "0" + ss2;
									while ((int)ss4.size() < 3) ss4 = "0" + ss4;
									s += ss1;
									s += ss2;
									s += ss3;
									s += ss4;
								}
								s+=";";
                                if (bombt) s+="1"; else s+="0"; s+=";";
                                if (bult) s+="1"; else s+="0"; s+=";";
				stemp=to_string(p1.HP);
                                for (int i=0;i<(3-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp; s+=";";
				stemp=to_string(p1.score);
                                for (int i=0;i<(3-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp; s+=";";
				stemp=to_string(p1.cbomb);
                                for (int i=0;i<(2-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp; s+=";";
				stemp=to_string(p1.carrow);
                                for (int i=0;i<(2-stemp.size());++i){
                                        s+="0";
                                }
                                s+=stemp;
                                char const *t=s.c_str();
                                send(new_socket,t,1024,0);
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
				for (int i = 0; i < 25; ++i) {
                                        for (int j = 0; j < 25; ++j) {
                                                if (things[i][j].size() != 0) {
                                                        RenderBox(lb, render, i, j);
                                                }
                                        }
                                }
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
						if (pl[it - ehit.begin()] == 1) ++p1.score;
						else ++p2.score;
						int surprise = gift();
						things[etemp[i].first / 40][(etemp[i].second + 39) / 40].push_back(surprise);
						tokill.push_back(en[i]);
					}
				}
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
