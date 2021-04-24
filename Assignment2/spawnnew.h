#include<bits/stdc++.h>
#include "maze.h"
using namespace std;
vector<int>enemy(700);
vector<pair<int, int>> randspawn(int x1, int y1, int x2, int y2)  // random spawn in each quarter
{
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
			if (!cell[i][j]) continue;
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
	x1 += 39;
	x2 += 39;
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
			return { 40 * (avail[i].second % 25) , 40 * (avail[i].second / 25) };
		}
	}
	return { -1,-1 };
}
vector<pair<int, int>> move(int x1, int y1, int x2, int y2, vector<pair<int, int>>cur, vector<int>dir)  // moves all enemies
{
	vector<pair<int, int>>loc;
	vector<int>exist(630);
	x1 /= 40;
	x2 /= 40;
	y1 += 39;
	y2 += 39;
	y1 /= 40;
	y2 /= 40;
	for (int i = 0; i < cur.size(); ++i)
	{
		if (cur[i].first % 40 != 0)
		{
			int pos1=cur[i].first/40,pos2=pos1+1,ypos=cur[i].second/40;
			if(min(dis[pos1+25*ypos][25*y1+x1],dis[pos1+25*ypos][25*y2+x2])<min(dis[pos2+25*ypos][25*y1+x1],dis[pos2+25*ypos][25*y2+x2])) loc.push_back({cur[i].first-10,cur[i].second});
			else loc.push_back({cur[i].first+10,cur[i].second});
			continue;
		}
		if (cur[i].second % 40 != 0)
		{
			int pos1=cur[i].second/40,pos2=pos1+1,xpos=cur[i].first/40;
			if(min(dis[25*pos1+xpos][25*y1+x1],dis[25*pos1+xpos][25*y2+x2])<min(dis[25*pos2+xpos][25*y1+x1],dis[25*pos2+xpos][25*y2+x2])) loc.push_back({cur[i].first,cur[i].second-10});
			else loc.push_back({cur[i].first,cur[i].second+10});
			continue;
		}
		int x = cur[i].first / 40, y = cur[i].second / 40, d=1000000, go=-1;
		int block = 25 * y + x;
		int follow;
		if (dis[block][25 * y1 + x1] < dis[block][25 * y2 + x2]) follow = 25 * y1 + x1;
		else if (dis[block][25 * y1 + x1] > dis[block][25 * y2 + x2]) follow = 25 * y2 + x2;
		else
		{
			int rr = rand() % 2;
			if (rr == 0) follow = 25 * y1 + x1;
			else follow = 25 * y2 + x2;
		}
		if (x > 0 && dis[block - 1][follow] < d)
		{
			d = dis[block - 1][follow];
			go = 4;
		}
		if (x < 24 && dis[block + 1][follow] < d)
		{
			d = dis[block + 1][follow];
			go = 2;
		}
		if (y > 0 && dis[block - 25][follow] < d)
		{
			d = dis[block - 25][follow];
			go = 1;
		}
		if (y < 24 && dis[block + 25][follow] < d)
		{
			d = dis[block + 25][follow];
			go = 3;
		}
		//cout<<follow<<' '<<d<<' '<<go<<' '<<x<<' '<<y<<'\n';
		int pixx = cur[i].first, pixy = cur[i].second;
		if (go == 1) pixy -= 10;
		else if (go == 2) pixx += 10;
		else if (go == 3) pixy += 10;
		else pixx -= 10;
		int nx = pixx / 40, ny = pixy / 40;
		//if(d==1) exit(0);
		if (d == 0 || (nx == x1 && ny == y1) || (nx == x2 && ny == y2) || exist[25 * ny + nx])
			loc.push_back(cur[i]);
		else
		{
			if (go==1) loc.push_back({ cur[i].first,cur[i].second - 10 });
			else if (go == 2) loc.push_back({ cur[i].first + 10, cur[i].second });
			else if (go == 3) loc.push_back({ cur[i].first, cur[i].second + 10 });
			else loc.push_back({ cur[i].first - 10, cur[i].second });
		}

	}
	return loc;
}
/*int main()
{
	MazeGenerate();
	for(int i=0;i<25;++i)
	{
		for(int j=0;j<25;++j)
		{
			if(cell[i][j]) cout<<1; else cout<<0; cout<<' ';
		}
		cout<<'\n';
	}
	vector<pair<int,int>>loc=randspawn(40,40,920,920);
	loc.pop_back();
	loc.pop_back();
	loc.pop_back();
	for(int i=0;i<1000;++i)
	{
		loc=move(40,40,920,920,loc,{-1,-1,-1,-1});
		for(int i=0;i<loc.size();++i)
		{
			cout<<loc[i].first<<' '<<loc[i].second<<'\n';
		}
	}
}*/