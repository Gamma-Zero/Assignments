#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8080

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 1000;
const int SPRITE = 40;
const int FPS = 60;

int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
int sock = 0, valreadc;
struct sockaddr_in serv_addr;
char bufferc[1024] = {0};
char buffer[1024] = {0};
SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* render=NULL;
SDL_Texture* wall=NULL;
SDL_Texture* arrow=NULL;
SDL_Texture* lb=NULL;
TTF_Font *font=NULL;
vector<vector<bool>> maze;
vector<Bomb> bombs;
vector<bullet>bul;
vector<Enemy> en;
vector<Enemy> tokill;
vector<int> ehit, pl;
vector<vector<vector<int>>>things(25, vector<vector<int>>(25));
int frame=0;
int eid=0;
int schedule=-1;
Mix_Music *bgm;
Mix_Chunk *bombsound, *bowsound;
