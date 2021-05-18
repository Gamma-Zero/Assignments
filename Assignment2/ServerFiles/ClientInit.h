#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <bits/stdc++.h>
#include "parse.h"
#include "Bomb.h"
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

