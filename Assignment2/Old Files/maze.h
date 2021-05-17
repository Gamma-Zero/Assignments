#include<bits/stdc++.h>
using namespace std;
vector<vector<bool>>cell(25, vector<bool>(25, 0));
vector<pair<int, int>>edge;
vector<int>num(700), parent(700), frcell;
vector<vector<int>>dis(700, vector<int>(700, 1000000));
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
		if (r < 24 && cell[r + 1][c] == 1)
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
	for (int i = 0; i < 625; i++) BFS(i);
}

vector<vector<bool>> MazeGenerate()
{
	Maze();
	for (int i = 0; i < 25; ++i)
	{
		for (int j = i + 1; j < 25; ++j)
			swap(cell[i][j], cell[j][i]);
	}
	return cell;
}
