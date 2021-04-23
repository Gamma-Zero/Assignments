#include<bits/stdc++.h>
#include "maze.h"
using namespace std;
vector<int>enemy(700);
vector<pair<int,int>> randspawn(int r1, int c1, int r2, int c2)  // random spawn in each quarter
{
	vector<pair<int, int>>loc;
	int p1 = 25 * r1 + c1, p2 = 25 * r2 + c2;
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
			loc.push_back({ z1 / 25,z1 % 25 });
			enemy[z1] = 1;
			break;
		}
	}
	while (true)
	{
		int y1 = rand() % tr.size(), z1 = tr[y1];
		if (dis[z1][p1] > 5 && dis[z1][p2] > 5)
		{
			loc.push_back({ z1 / 25,z1 % 25 });
			enemy[z1] = 1;
			break;
		}
	}
	while (true)
	{
		int y1 = rand() % bl.size(), z1 = bl[y1];
		if (dis[z1][p1] > 5 && dis[z1][p2] > 5)
		{
			loc.push_back({ z1 / 25,z1 % 25 });
			enemy[z1] = 1;
			break;
		}
	}
	while (true)
	{
		int y1 = rand() % br.size(), z1 = br[y1];
		if (dis[z1][p1] > 5 && dis[z1][p2] > 5)
		{
			loc.push_back({ z1 / 25,z1 % 25 });
			enemy[z1] = 1;
			break;
		}
	}
	return loc;
}
pair<int,int> pspawn(int r1, int c1, int r2, int c2) // periodic spawn
{
	vector<int>fr1;
	vector<pair<int,int>>avail;
	for (int i = 0; i < frcell.size(); ++i)
	{
		if (dis[frcell[i]][25*r1+c1]<=5 || dis[frcell[i]][25 * r2 + c2] <= 5 || enemy[frcell[i]] == 1) continue;
		int block = frcell[i],weight=dis[block][25*r1+c1]*dis[block][25*r2+c2];
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
			return { avail[i].second / 25, avail[i].second % 25 };
		}
	}
	return { -1,-1 };
}
vector<pair<int, int>> move(int r1, int c1, int r2, int c2)  // moves all enemies
{
	vector<int>enemy1(700);
	vector<pair<int, int>>loc;
	for (int i = 0; i < 700; ++i)
	{
		if (enemy[i] == 0) continue;
		int follow;
		if (dis[i][25 * r1 + c1] < dis[i][25 * r2 + c2]) follow = 25 * r1 + c1;
		else if (dis[i][25 * r1 + c1] > dis[i][25 * r2 + c2]) follow = 25 * r2 + c2;
		else
		{
			int r = rand() % 2;
			if (r == 0) follow = 25 * r1 + c1;
			else  follow = 25 * r2 + c2;
		}
		int r = i / 25, c = i % 25,nr=-1,nc=-1,d=1000000;
		if (r > 0 && dis[i - 25][follow] < d)
		{
			d = dis[i - 25][follow];
			nr = r - 1;
			nc = c;
		}
		if (r < 24 && dis[i + 25][follow] < d)
		{
			d = dis[i + 25][follow];
			nr = r + 1;
			nc = c;
		}
		if (c > 0 && dis[i - 1][follow] < d)
		{
			d = dis[i - 1][follow];
			nr = r;
			nc = c - 1;
		}
		if (c < 24 && dis[i + 1][follow] < d)
		{
			d = dis[i + 1][follow];
			nr = r;
			nc = c + 1;
		}
		loc.push_back({ nr,nc });
		enemy1[25 * nr + nc] = 1;

	}
	enemy = enemy1;
	return loc;
}
