#include<bits/stdc++.h>
using namespace std;
vector<vector<int>>cell(50, vector<int>(50, 0));
vector<pair<int, int>>edge, unused;
vector<int>num(2510), parent(2510), target;
vector<vector<int>>dis(2510, vector<int>(2510, 1000000));
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
	if (u == v)
	{
		unused.push_back({ u,v });
		return;
	}
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
	int r = change / 50, c = change % 50;
	cell[r][c] = 1;
}

vector<pair<int,int>> Path(int x1, int y1, int x2, int y2)
{
	vector<pair<int, int>>ret;
	int c1 = 50 * y1 + x1, c2 = 50 * y2 + x2;
	int distance = dis[c1][c2];
	while (distance > 0)
	{
		ret.push_back({ c2 % 50, c2 / 50 });
		int cx = c2 % 50, cy = c2 / 50, cdis = 100000000, nx = -1, ny = -1;
		if (cx > 0 && dis[c1][c2 - 1] < distance)
		{
			distance = dis[c1][c2 - 1];
			nx = cx - 1;
			ny = cy;
		}
		if (cy > 0 && dis[c1][c2 - 50] < distance)
		{
			distance = dis[c1][c2 - 50];
			nx = cx;
			ny = cy - 1;
		}
		if (cx < 49 && dis[c1][c2 + 1] < distance)
		{
			distance = dis[c1][c2 + 1];
			nx = cx + 1;
			ny = cy;
		}
		if (cy < 49 && dis[c1][c2 + 50] < distance)
		{
			distance = dis[c1][c2 + 50];
			nx = cx;
			ny = cy +1;
		}
		c2 = 50 * ny + nx;

	}
	ret.push_back({ x1,y1 });
	reverse(ret.begin(), ret.end());
	return ret;
}

void shortestPath()
{
	for (int i = 0; i < 2500; ++i)
	{
		int curx = i / 50, cury = i % 50;
		swap(curx, cury);
		dis[i][i] = 0;
		set<pair<int, int>>s;
		for (int j = 0; j < 2500; ++j) s.insert({ dis[i][j],j });
		while (!s.empty())
		{
			auto it = s.begin();
			int cur = it->second, len = it->first, x = cur % 50, y = cur / 50;
			s.erase(it);
			if (len == 1000000) break;
			if (x < 49)
			{
				int add;
				if (cell[x + 1][y] == 1) add = 1; else if (cell[x + 1][y] == 2) add = 11; else add = 1000000;
				if (len + add < dis[i][cur + 1])
				{
					s.erase({ dis[i][cur + 1], cur + 1 });
					dis[i][cur + 1] = len + add;
					s.insert({ dis[i][cur + 1], cur + 1 });
				}
			}
			if (x > 0)
			{
				int add;
				if (cell[x - 1][y] == 1) add = 1; else if (cell[x - 1][y] == 2) add = 11; else add = 1000000;
				if (len + add < dis[i][cur - 1])
				{
					s.erase({ dis[i][cur - 1], cur - 1 });
					dis[i][cur - 1] = len + add;
					s.insert({ dis[i][cur - 1], cur - 1 });
				}
			}
			if (y < 49)
			{
				int add;
				if (cell[x][y + 1] == 1) add = 1; else if (cell[x][y + 1] == 2) add = 11; else add = 1000000;
				if (len + add < dis[i][cur + 50])
				{
					s.erase({ dis[i][cur + 50], cur + 50 });
					dis[i][cur + 50] = len + add;
					s.insert({ dis[i][cur + 50], cur + 50 });
				}
			}
			if (y > 0)
			{
				int add;
				if (cell[x][y - 1] == 1) add = 1; else if (cell[x][y - 1] == 2) add = 11; else add = 1000000;
				if (len + add < dis[i][cur - 50])
				{
					s.erase({ dis[i][cur - 50], cur - 50});
					dis[i][cur - 50] = len + add;
					s.insert({ dis[i][cur - 50], cur - 50 });
				}
			}
		}
	}
}


void Maze()
{
	for (int i = 1; i < 50; i += 2) for (int j = 1; j < 50; j += 2) cell[i][j] = 1;
	for (int i = 1; i < 50; i += 2)
	{
		for (int j = 1; j < 50; j += 2)
		{
			int cur = 50 * i + j;
			num[cur] = 1;
			parent[cur] = cur;
			if (j > 1) edge.push_back({ cur - 2,cur });
			if (j < 48) edge.push_back({ cur,cur + 2 });
			if (i > 1) edge.push_back({ cur - 100,cur });
			if (i < 48) edge.push_back({ cur,cur + 100 });
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
	for (int i = 0; i < 50; ++i)
	{
		for (int j = i + 1; j < 50; ++j)
			swap(cell[i][j], cell[j][i]);
	}
	for (int i = 0; i < 50; i+=10)
	{
		for (int j = 0; j < 50; j+=10)
		{
			vector<pair<int, int>>frcell;
			for (int x = i; x < i + 10; ++x)
			{
				for (int y = j; y < j + 10; ++y)
					if (cell[x][y]) frcell.push_back({ x,y });
			}
			int sel = rand() % frcell.size();
			target.push_back(50*frcell[sel].second+frcell[sel].first);
			for (int k = 0; k < 5; ++k)
			{
				srand(time(0));
				int ind = rand() % frcell.size();
				auto index = frcell[ind];
				cell[index.first][index.second] = 2;
				vector<pair<int, int>>fr1;
				for (int w = 0; w < frcell.size(); ++w)
				{
					if (frcell[w] == index) continue;
					fr1.push_back(frcell[w]);
				}
				frcell = fr1;
			}
		}
	}
	srand(time(0));
	random_shuffle(target.begin(), target.end());
	for (int i = 0; i < 5; ++i) target.pop_back();
	shortestPath();
}

vector<vector<int>> MazeGenerate()
{
	Maze();
	return cell;
}