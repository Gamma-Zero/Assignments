#include <bits/stdc++.h>

using namespace std;

bool CollisionMaze(int x, int y, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SPRITE, vector<vector<bool>> mat, vector<pair<int,int>> en){
	if (x<0 || y<0 || x>(SCREEN_WIDTH-SPRITE) || y>(SCREEN_HEIGHT-SPRITE)){
		return true;
	}
	if (!mat[x/SPRITE][(y+SPRITE-1)/SPRITE] || !mat[(x+SPRITE-1)/SPRITE][(y+SPRITE-1)/SPRITE]){
		return true;
	}
	for (auto i:en){
		if ((i.first/SPRITE==x/SPRITE) && ((i.second+39)/SPRITE==(y+39)/SPRITE)){
			return true;
		}
	}
	return false;
}

bool CollisionBomb(int x, int y, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SPRITE, vector<vector<bool>> mat){
	if (x<0 || y<0 || x>(SCREEN_WIDTH-SPRITE) || y>(SCREEN_HEIGHT-SPRITE)){
                return true;
        }
	if (!mat[x/SPRITE][y/SPRITE]){
		return true;
	}
	return false;
}
