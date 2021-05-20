#include<bits/stdc++.h>
#include "maze.h"
#include "Loaders.h"
using namespace std;
int main()
{
	init();
	MazeGenerate();
	SDL_Texture* wall=loadTexture(loadPNG("Textures/wall.png"));
	SDL_Texture* bg=loadTexture(loadPNG("Textures/grass.png"));
	SDL_Texture* broken=loadTexture(loadPNG("Textures/broken.png"));
	SDL_Texture* chest=loadTexture(loadPNG("Textures/chest.png"));
	SDL_Texture* cross=loadTexture(loadPNG("Textures/cross.png"));
	vector<pair<int, int>>free;
	for (int i = 0; i < 50; ++i) for (int j = 0; j < 50; ++j) if (cell[i][j]) free.push_back({ i,j });
	int x = rand() % free.size();
	int xs = free[x].first, ys = free[x].second;
	target.push_back(50*ys+xs);
	reverse(target.begin(), target.end());
	target.push_back(50*ys+xs);
	int length = 0;
	for (int i = 0; i < (int)target.size() - 1; ++i)
		length += dis[target[i]][target[i+1]];
	int sz = target.size() - 2;
	srand(time(0));
	float temp = 1.0, factor=0.9, k=50.0;
	cout << length << '\n';
	bool quit=false;
	while (!quit){
	for (int i = 1; i <= 10; ++i)   // cooling steps
	{
		temp *= factor;
		for (int j = 1; j <= 100; ++j)
		{
			SDL_Event e;
			while(SDL_PollEvent(&e)){
				if (e.type==SDL_QUIT){
					quit=true;
				}
			}
			SDL_Rect space={0,0,1000,1000};
			SDL_RenderCopy(render,bg,NULL,&space);
			for (int k=0;k<50;++k){
				for (int l=0;l<50;++l){
					if (cell[k][l]==0){
						SDL_Rect temp={20*k,20*l,20,20};
						SDL_RenderCopy(render,wall,NULL,&temp);
					}
					if (cell[k][l]==2){
						SDL_Rect temp={20*k,20*l,20,20};
						SDL_RenderCopy(render,broken,NULL,&temp);
					}
				}
			}
			int take1 = 1 + rand() % sz;
			int take2 = 1 + rand() % sz;
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			int length2 = 0;
			swap(target[take2], target[take1]);
			for (int ii=1;ii<target.size();++ii){
		//		cout << target[ii] << " ";
				auto p=Path(target[ii-1]%50,target[ii-1]/50,target[ii]%50,target[ii]/50);
				for (auto ij:p){
					SDL_Rect temp={20*ij.first,20*ij.second,20,20};
                                        SDL_RenderCopy(render,cross,NULL,&temp);
				}
			}
		//	cout << '\n';
			for (auto ii:target){
                                int x=ii%50; int y=ii/50;
                                SDL_Rect temp={20*x,20*y,20,20};
                                SDL_RenderCopy(render,chest,NULL,&temp);
                        }
                        for (int w = 0; w < (int)target.size() - 1; ++w)
                                length2 += dis[target[w]][target[w + 1]];
                        swap(target[take2], target[take1]);
			float exponent = (length - length2) / (k * temp);
			float power = exp(exponent);
			if (length2 < length)
				swap(target[take1], target[take2]);
			else if (power > r)
				swap(target[take1], target[take2]);
			length = min(length, length2);
			int val = 0;
			//cout << take1 << ' ' << take2 << ' '<<length<<'\n';
			SDL_Delay(1000/12);
			SDL_RenderPresent(render);
			SDL_RenderClear(render);
			if (quit){
				break;
			}
		}
		if (quit){
			break;
		}
	}
	quit=true;
	}
	cout << length << '\n';
}
