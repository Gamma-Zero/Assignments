#include<bits/stdc++.h>
#include "maze.h"
using namespace std;
vector<int>enemy(700);
vector<pair<int, int>> randspawn(int x1, int y1, int x2, int y2)  // random spawn in each quarter
{
	y1 += 39;
	y2 += 39;
	x1 /= 40;
	x2 /= 40;
	y1 /= 40;
	y2 /= 40;
	vector<pair<int, int>>loc;
	int p1 = 25 * y1 + x1, p2 = 25 * y2 + x2;
	vector<int>tl, tr, bl, br;
	for (int i = 0; i < 25; ++i)
	{
		for (int j = 0; j < 25; ++j)
		{
			if (cell[i][j] == 0) continue;
			if (i < 13 && j < 12) tl.push_back(25 * i + j);
			else if (i >= 13 && j < 13) bl.push_back(25 * i + j);
			else if (i < 12 && j >= 12) tr.push_back(25 * i + j);
			else br.push_back(25 * i + j);
		}
	}
	srand(time(0));
	while (true)
	{
		int y1 = rand() % tl.size(), z1 = tl[y1];
		if (dis[z1][p1] > 5 && dis[z1][p2] > 5)
		{
			loc.push_back({ 40 * (z1 % 25), 40 * (z1 / 25) });
			enemy[z1] = 1;
			break;
		}
	}
	while (true)
	{
		int y1 = rand() % tr.size(), z1 = tr[y1];
		if (dis[z1][p1] > 5 && dis[z1][p2] > 5)
		{
			loc.push_back({ 40 * (z1 % 25) , 40 * (z1 / 25) });
			enemy[z1] = 1;
			break;
		}
	}
	while (true)
	{
		int y1 = rand() % bl.size(), z1 = bl[y1];
		if (dis[z1][p1] > 5 && dis[z1][p2] > 5)
		{
			loc.push_back({ 40 * (z1 % 25) , 40 * (z1 / 25) });
			enemy[z1] = 1;
			break;
		}
	}
	while (true)
	{
		int y1 = rand() % br.size(), z1 = br[y1];
		if (dis[z1][p1] > 5 && dis[z1][p2] > 5)
		{
			loc.push_back({ 40 * (z1 % 25) , 40 * (z1 / 25) });
			enemy[z1] = 1;
			break;
		}
	}
	return loc;
}
pair<int, int> pspawn(int x1, int y1, int x2, int y2, vector<pair<int, int>>cur) // periodic spawn
{
	y1 += 39;
	y2 += 39;
	x1 /= 40;
	x2 /= 40;
	y1 /= 40;
	y2 /= 40;
	enemy.assign(700, 0);
	for (int i = 0; i < cur.size(); ++i) enemy[25 * (cur[i].second / 40) + (cur[i].first / 40)] = 1;
	vector<int>fr1;
	vector<pair<int, int>>avail;
	for (int i = 0; i < frcell.size(); ++i)
	{
		if (dis[frcell[i]][25 * y1 + x1] <= 5 || dis[frcell[i]][25 * y2 + x2] <= 5 || enemy[frcell[i]] == 1) continue;
		int block = frcell[i], weight = dis[block][25 * y1 + x1] * dis[block][25 * y2 + x2];
		avail.push_back({ weight,block });
	}
	sort(avail.begin(), avail.end());
	for (int i = 1; i < avail.size(); ++i) avail[i].first += avail[i - 1].first;
	int r = rand() % (avail.back().first); ++r;
	for (int i = 0; i < avail.size(); ++i)
	{
		if (r <= avail[i].first)
		{
			enemy[avail[i].second] = 1;
			return { 40 * (avail[i].second % 25) , 40 * (avail[i].second / 25)  };
		}
	}
	return { -1,-1 };
}
vector<pair<int, int>> move(int x1, int y1, int x2, int y2, vector<pair<int, int>>cur)  // moves all enemies
{
	y1 += 39;
	y2 += 39;
	y1 /= 40;
	y2 /= 40;
	x1 /= 40;
	x2 /= 40;
	vector<pair<int, int>>loc;
	vector<int>nen(700);
	for (int i = 0; i < cur.size(); ++i)
	{
		int curcell = 25 * (cur[i].second / 40) + (cur[i].first / 40);
		int follow;
		if (dis[curcell][25 * y1 + x1] < dis[curcell][25 * y2 + x2]) follow = 25 * y1 + x1;
		else if (dis[curcell][25 * y1 + x1] > dis[curcell][25 * y2 + x2]) follow = 25 * y2 + x2;
		else
		{
			int r = rand() % 2;
			if (r == 0) follow = 25 * y1 + x1;
			else  follow = 25 * y2 + x2;
		}
		//cout<<follow<<' '<<cur[i].first<<' '<<cur[i].second<<' '<<curcell<<' '<<dis[curcell][25 * r1 + c1]<<'\n';
		int y = curcell / 25, x = curcell % 25, nx = -1, ny = -1, d = 1000000, dir = -1;
		if (y > 0 && dis[curcell - 25][follow] < d)
		{
			d = dis[curcell - 25][follow];
			dir = 4;
		}
		if (y < 24 && dis[curcell + 25][follow] < d)
		{
			d = dis[curcell + 25][follow];
			dir = 2;
		}
		if (x > 0 && dis[curcell - 1][follow] < d)
		{
			d = dis[curcell - 1][follow];
			dir = 3;
		}
		if (x < 24 && dis[curcell + 1][follow] < d)
		{
			d = dis[curcell + 1][follow];
			dir = 1;
		}
		int pixelx = cur[i].first, pixely = cur[i].second;
		if (dir == 1) pixelx += 10; else if (dir == 2) pixely += 10; else if (dir == 3) pixelx -= 10; else pixely -= 10;
		nx = pixelx / 40;
		ny = pixely / 40;
		if ((nx == x1 && ny == y1) || (nx == x2 && ny == y2) || nen[25 * ny + nx] == 1)
			loc.push_back(cur[i]);
		else
		{
			nen[25 * ny + nx] = 1;
			if (dir == 1) loc.push_back({ cur[i].first + 10, cur[i].second });
			else if (dir == 2) loc.push_back({ cur[i].first , cur[i].second + 10});
			else if (dir == 3) loc.push_back({ cur[i].first - 10, cur[i].second });
			else loc.push_back({ cur[i].first , cur[i].second - 10 });
		}

	}
	return loc;
}
