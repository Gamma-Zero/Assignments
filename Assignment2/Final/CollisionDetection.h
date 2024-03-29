#include <bits/stdc++.h>

using namespace std;

bool CollisionMaze(int x, int y, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SPRITE, vector<vector<bool>> mat, vector<pair<int, int>> en) {
	if (x<0 || y<0 || x>(SCREEN_WIDTH - SPRITE) || y>(SCREEN_HEIGHT - SPRITE)) {
		return true;
	}
	if (!mat[x / SPRITE][(y + SPRITE - 1) / SPRITE] || !mat[(x + SPRITE - 1) / SPRITE][(y + SPRITE - 1) / SPRITE]) {
		return true;
	}
	for (auto i : en) {
		if ((x < i.first + 40) && (x + 40 > i.first) &&
			(y < i.second + 40) &&
			(y + 40 > i.second)) {
			return true;
		}
	}
	return false;
}

bool CollisionBomb(int x, int y, int SCREEN_WIDTH, int SCREEN_HEIGHT, int SPRITE, vector<vector<bool>> mat) {
	if (x<0 || y<0 || x>(SCREEN_WIDTH - SPRITE) || y>(SCREEN_HEIGHT - SPRITE)) {
		return true;
	}
	if (!mat[x / SPRITE][y / SPRITE]) {
		return true;
	}
	return false;
}

bool CollisionP1P2(int x1, int y1, int x2, int y2)
{
	x1 /= 40;
	x2 /= 40;
	y1 += 39;
	y2 += 39;
	y1 /= 40;
	y2 /= 40;
	if (x1 == x2 && y1 == y2) return true;
	else return false;
}
