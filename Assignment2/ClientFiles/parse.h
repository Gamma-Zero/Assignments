#include<bits/stdc++.h>
using namespace std;
vector<int> parse(string s){    // player(9 char, x+y+dir+ismove+anim);enemy(12 char, x+y+dir+id);bomb(1 char);arrow(1 char)
    int index=12;
    string s1=s.substr(0,4), s2=s.substr(4,4), s3=s.substr(8,1),s4=s.substr(9,1), s5=s.substr(10,1);
    stringstream p1(s1), p2(s2), p3(s3), p4(s4), p5(s5);
    int w1,w2,w3,w4,w5;
    p1>>w1;
    p2>>w2;
    p3>>w3;
    p4>>w4;
    p5>>w5;
    vector<int>v;
    v.push_back(w1);
    v.push_back(w2);
    v.push_back(w3);
    v.push_back(w4);
    v.push_back(w5);
    w1=s.back()-'0';
    w2=s[(int)s.size() - 3]-'0';
    v.push_back(w2);
    v.push_back(w1);
    while(index<(int)s.size()-4){
        s1=s.substr(index,4);
        index+=4;
        s2=s.substr(index,4);
        index+=4;
        s3=s[index];
        ++index;
        s4=s.substr(index,3);
        index+=3;
        stringstream q1(s1),q2(s2), q3(s3), q4(s4);
        q1>>w1;
        q2>>w2;
        q3>>w3;
        q4>>w4;
        v.push_back(w1);
        v.push_back(w2);
        v.push_back(w3);
        v.push_back(w4);

    }
    return v;   // player(x+y+dir+ismove+anim),bomb?,arrow?,enemy(x+y+dir+id)
}
