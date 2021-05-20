#include<bits/stdc++.h>
#include "maze.h"
using namespace std;
int main()
{
	MazeGenerate();
	vector<pair<int, int>>free;
	for (int i = 0; i < 50; ++i) for (int j = 0; j < 50; ++j) if (cell[i][j]) free.push_back({ i,j });
	int x = rand() % free.size();
	int xs = free[x].first, ys = free[x].second;
	target.push_back(50*ys+xs);
	reverse(target.begin(), target.end());
	target.push_back(50*ys+xs);
	int length = 0;
	for (int i = 0; i < (int)target.size() - 1; ++i)
		length += dis[target[i]][target[i+1]];
	int sz = target.size() - 2;
	srand(time(0));
	float temp = 1.0, factor=0.9, k=50.0;
	cout << length << '\n';
	for (int i = 1; i <= 1000; ++i)   // cooling steps
	{
		temp *= factor;
		for (int j = 1; j <= 1000; ++j)
		{
			int take1 = 1 + rand() % sz;
			int take2 = 1 + rand() % sz;
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			int length2 = 0;
			swap(target[take2], target[take1]);
			for (int w = 0; w < (int)target.size() - 1; ++w)
				length2 += dis[target[w]][target[w + 1]];
			swap(target[take2], target[take1]);
			float exponent = (length - length2) / (k * temp);
			float power = exp(exponent);
			if (length2 < length)
				swap(target[take1], target[take2]);
			else if (power > r)
				swap(target[take1], target[take2]);
			length = min(length, length2);
			int val = 0;
			//cout << take1 << ' ' << take2 << ' '<<length<<'\n';
		}
	}
	cout << length;
}