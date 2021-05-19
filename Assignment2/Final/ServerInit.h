#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <bits/stdc++.h>
#include "parse.h"
#include "Bomb.h"
#include "Global.h"
#include "font.h"

bool init(string ip, string port){
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
					maze = MazeGenerate();
					SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
					screenSurface = SDL_GetWindowSurface(window);
					font = TTF_OpenFont("Fonts/Raleway-Black.ttf", 20);
					string idle="Waiting for client...";
                                        SDL_Color fcolor={173,216,230};
                                        SDL_Color bcolor={0,0,0};
                                        SDL_Surface* temp=TTF_RenderText_Shaded(font,idle.c_str(),fcolor,bcolor);
					SDL_Texture* tx=loadTexture(temp);
                                        SDL_Rect space={500,500,10*idle.size(),20};
                                        SDL_RenderCopy(render,tx,NULL,&space);
                                        SDL_RenderPresent(render);
					if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
                                        {
                                                perror("socket failed");
                                                exit(EXIT_FAILURE);
                                        }
                                        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
                                        {
                                                perror("setsockopt");
                                                exit(EXIT_FAILURE);
                                        }
                                        address.sin_family = AF_INET;
                                        address.sin_addr.s_addr = inet_addr(ip.c_str());
                                        address.sin_port = htons(stoi(port));
                                        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
                                        {
                                                perror("bind failed");
                                                exit(EXIT_FAILURE);
                                        }
                                        if (listen(server_fd, 3) < 0)
                                        {
                                                perror("listen");
                                                exit(EXIT_FAILURE);
                                        }
                                        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
                                        {
                                                perror("accept");
                                                exit(EXIT_FAILURE);
                                        }
                                        string t="";
                                        for(int i=0;i<25;++i){
                                                for (int j=0;j<25;++j){
                                                        if (maze[i][j]==0){
                                                                t+="0";
                                                        } else {
                                                                t+="1";
                                                        }
                                                }
                                        }
                                        char const *s=t.c_str();
                                        send(new_socket,s,1024,0);
				}
			}
		}
	}
	BFS(maze);
	return success;
}
