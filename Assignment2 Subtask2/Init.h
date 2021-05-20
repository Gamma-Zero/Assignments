#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Global.h"

bool init(){
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
		TTF_Init();
		window = SDL_CreateWindow("Simulated Annealing Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 1000, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
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
					SDL_SetRenderDrawColor(render, 0, 0, 0, 0);
					screenSurface = SDL_GetWindowSurface(window);
					font = TTF_OpenFont("Fonts/Raleway-Black.ttf", 20);
				}
			}
		}
	}
	return success;
}
