#include<bits/stdc++.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
vector<Point2f>cor_init;
void Click(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        cor_init.push_back(Point2f(x, y));
    }
}
int main()
{
    ofstream file2;
    file2.open("cor.csv");
    Mat img;
    VideoCapture cap2("vid.mp4");
    cap2 >> img;
    cvtColor(img, img, COLOR_BGR2GRAY);
    namedWindow("Display", WINDOW_NORMAL);
    setMouseCallback("Display", Click, NULL);
    imshow("Display", img);
    waitKey(0);
    for (int i = 0; i < cor_init.size(); ++i)
        file2 << cor_init[i].x << "," << cor_init[i].y << "\n";
    file2.close();
}
