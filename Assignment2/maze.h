#include<bits/stdc++.h>
using namespace std;
vector<vector<bool>>cell(25, vector<bool>(25, 0));
vector<pair<int, int>>edge;
vector<int>num(700), parent(700),frcell,enemy(700);
vector<vector<int>>dis(700, vector<int>(700,1000000));
int find(int a)
{
	if (a == parent[a]) return a;
	return parent[a] = find(parent[a]);
}
void join(int a, int b)
{
	//cout << a << ' ' << b << '\n';
	int u = find(a);
	int v = find(b);
	if (u == v) return;
	if (num[u] < num[v])
	{
		parent[u] = v;
		num[v] += num[u];
	}
	else
	{
		parent[v] = u;
		num[u] += num[v];
	}
	int change = (a + b) / 2;
	int r = change / 25, c = change % 25;
	cell[r][c] = 1;
}
void BFS(int cur)
{
	if (cell[cur / 25][cur % 25] == 0) return;
	int r, c;
	queue<int>q;
	dis[cur][cur] = 0;
	q.push(cur);
	while (!q.empty())
	{
		int x = q.front(); q.pop();
		r = x / 25; c = x % 25;
		if (r < 24 && cell[r+1][c]==1)
		{
			if (dis[cur][r * 25 + c + 25] > dis[cur][x] + 1)
			{
				q.push(r * 25 + c + 25);
				dis[cur][r * 25 + c + 25] = dis[cur][x] + 1;
			}
		}
		if (r > 0 && cell[r - 1][c] == 1)
		{
			if (dis[cur][r * 25 + c - 25] > dis[cur][x] + 1)
			{
				q.push(r * 25 + c - 25);
				dis[cur][r * 25 + c - 25] = dis[cur][x] + 1;
			}
		}
		if (c < 24 && cell[r][c + 1] == 1)
		{
			if (dis[cur][r * 25 + c + 1] > dis[cur][x] + 1)
			{
				q.push(r * 25 + c + 1);
				dis[cur][r * 25 + c + 1] = dis[cur][x] + 1;
			}
		}
		if (c > 0 && cell[r][c - 1] == 1)
		{
			if (dis[cur][r * 25 + c - 1] > dis[cur][x] + 1)
			{
				q.push(r * 25 + c - 1);
				dis[cur][r * 25 + c - 1] = dis[cur][x] + 1;
			}
		}
	}
}
void Maze()
{
	for (int i = 1; i < 25; i += 2) for (int j = 1; j < 25; j += 2) cell[i][j] = 1;
	for (int i = 1; i < 25; i += 2)
	{
		for (int j = 1; j < 25; j += 2)
		{
			int cur = 25 * i + j;
			num[cur] = 1;
			parent[cur] = cur;
			if (j > 1) edge.push_back({ cur - 2,cur });
			if (j < 23) edge.push_back({ cur,cur + 2 });
			if (i > 1) edge.push_back({ cur - 50,cur });
			if (i < 23) edge.push_back({ cur,cur + 50 });
		}
	}
	srand(time(0));
	while (!edge.empty())
	{
		int s = edge.size();
		int index = rand() % s;
		join(edge[index].first, edge[index].second);
		vector<pair<int, int>>edge1;
		for (int i = 0; i < s; ++i)
		{
			if (i == index) continue;
			edge1.push_back(edge[i]);
		}
		edge = edge1;
	}
	for (int i = 0; i < 25; ++i) for (int j = 0; j < 25; ++j) if (cell[i][j]) frcell.push_back(25 * i + j);
	for (int i = 0; i < 625; ++i) BFS(i);
}

vector<vector<bool>> MazeGenerate() 
{
	Maze();
	return cell;
}
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
