#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Texture* loadTexture(SDL_Surface* input){
        Uint32 colorkey=SDL_MapRGB(input->format,0,0,0);
        SDL_SetColorKey(input,SDL_TRUE,colorkey);
        SDL_Texture* temp=SDL_CreateTextureFromSurface(render,input);
        if (temp==NULL){
                printf("Texture Conversion died\n");
        }
        return temp;
}

void RenderFont(SDL_Renderer* render, TTF_Font *font, int x, int y, int text){
	string t=to_string(text);
	SDL_Color fcolor={173,216,230};
	SDL_Color bcolor={0,0,0};
	SDL_Surface* temp=TTF_RenderText_Shaded(font,t.c_str(),fcolor,bcolor);
	SDL_Texture* tx=loadTexture(temp);
	SDL_Rect space={x,y,20,20};
	SDL_RenderCopy(render,tx,NULL,&space);
}

void RenderData(Player p1, Player p2, SDL_Renderer* render, TTF_Font *font){
	string t="Player 1:";
        SDL_Color fcolor={173,216,230};
        SDL_Color bcolor={0,0,0};
        SDL_Surface* temp=TTF_RenderText_Shaded(font,t.c_str(),fcolor,bcolor);
        SDL_Texture* tx=loadTexture(temp);
        SDL_Rect space={1000,300,10*t.size(),20};
        SDL_RenderCopy(render,tx,NULL,&space);
	t="Ammo count: "+to_string(p1.carrow);
	temp=TTF_RenderText_Shaded(font,t.c_str(),fcolor,bcolor);
        tx=loadTexture(temp);
        space={1000,350,10*t.size(),20};
        SDL_RenderCopy(render,tx,NULL,&space);
	t="Bomb count: "+to_string(p1.cbomb);
        temp=TTF_RenderText_Shaded(font,t.c_str(),fcolor,bcolor);
        tx=loadTexture(temp);
        space={1000,400,10*t.size(),20};
        SDL_RenderCopy(render,tx,NULL,&space);
	t="Score: "+to_string(p1.score);
        temp=TTF_RenderText_Shaded(font,t.c_str(),fcolor,bcolor);
        tx=loadTexture(temp);
        space={1000,450,10*t.size(),20};
        SDL_RenderCopy(render,tx,NULL,&space);
	t="Player 2:";
	temp=TTF_RenderText_Shaded(font,t.c_str(),fcolor,bcolor);
        tx=loadTexture(temp);
        space={1000,500,10*t.size(),20};
	SDL_RenderCopy(render,tx,NULL,&space);
	t="Ammo count: "+to_string(p2.carrow);
        temp=TTF_RenderText_Shaded(font,t.c_str(),fcolor,bcolor);
        tx=loadTexture(temp);
        space={1000,550,10*t.size(),20};
        SDL_RenderCopy(render,tx,NULL,&space);
        t="Bomb count: "+to_string(p2.cbomb);
        temp=TTF_RenderText_Shaded(font,t.c_str(),fcolor,bcolor);
        tx=loadTexture(temp);
        space={1000,600,10*t.size(),20};
        SDL_RenderCopy(render,tx,NULL,&space);
        t="Score: "+to_string(p2.score);
        temp=TTF_RenderText_Shaded(font,t.c_str(),fcolor,bcolor);
        tx=loadTexture(temp);
        space={1000,650,10*t.size(),20};
        SDL_RenderCopy(render,tx,NULL,&space);
}
