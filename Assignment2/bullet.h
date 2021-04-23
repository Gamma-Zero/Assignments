#include<bits/stdc++.h>
#include "maze.h"
using namespace std;
struct bullet
{
    int cover=0,dir,row,col;
};
bullet spblt(int r1, int c1,int direc)   // spawn bullet
{
    bullet b;
    b.dir=direc;
    b.row=r1;
    b.col=c1;
    return b;
}
pair<int,int> mvblt(vector<bullet>&b, vector<pair<int,int>>&en, int r1, int c1, int r2, int c2, int health1, int health2)
{
    vector<bullet>change;
    vector<int>enemy(700);
    for(int i=0;i<en.size();++i) enemy[25*en[i].first+en[i].second]=1;
    for(int i=0;i<b.size();++i)
    {
        if(b[i].dir==1) ++b[i].col;
        else if(b[i].dir==2) ++b[i].row;
        else if(b[i].dir==3) --b[i].col;
        else --b[i].row;
        ++b[i].cover;
        if(b[i].cover>5) continue;
        if(b[i].row<0 || b[i].row>24 || b[i].col<0 || b[i].col>24 || cell[b[i].row][b[i].col]==0) continue;
        if(enemy[25*b[i].row + b[i].col]==1)
        {
            enemy[25*b[i].row+b[i].col]=0;  // one-shot KO
            continue;
        }
        if(b[i].row==r1 && b[i].col==c1)
            health1-=10;  // can be changed
        else if(b[i].row==r2 && b[i].col==c2)
            health2-=10;  // can be changed
        else
            change.push_back(b[i]);
    }
    en.clear();
    for(int i=0;i<700;++i) if(enemy[i]) en.push_back({i/25,i%25});
    b=change;
    return make_pair(health1,health2);
}