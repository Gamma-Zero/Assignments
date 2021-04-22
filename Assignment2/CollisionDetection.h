#include <bits/stdc++.h>

using namespace std;

bool CollisionMaze(int x, int y, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SPRITE, vector<vector<bool>> mat){
	if (x<0 || y<0 || x>(SCREEN_WIDTH-SPRITE) || y>(SCREEN_HEIGHT-SPRITE)){
		return true;
	}
	x=x/SPRITE;
	y=y/SPRITE;
	if (!mat[x][y]){
		return true;
	}
	return false;
}
