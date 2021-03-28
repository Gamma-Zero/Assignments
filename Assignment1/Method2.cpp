#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;
vector<Point2f>cor_init, cor_fin;
double error1 = 0;
vector<double>baseline;
void Click(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        cor_init.push_back(Point2f(x, y));
    }
}
void get(int x,int y)
{
    VideoCapture cap2("vid.mp4");
    if (!cap2.isOpened())
    {
        cout << "Error, cannot open the video.";
        cin.get();
        return;
    }
    double fps = cap2.get(CAP_PROP_FPS);
    Mat frame, subt, img, change;
    if (x == 1)
    {
        cap2 >> img;
        cvtColor(img, img, COLOR_BGR2GRAY);
        namedWindow("Display", WINDOW_NORMAL);
        setMouseCallback("Display", Click, NULL);
        imshow("Display", img);
        waitKey(0);
    }
    change = findHomography(cor_init, cor_fin);
    long long framenum = 0;
    Mat empty = imread("empty2.png");
    cvtColor(empty, empty, COLOR_BGR2GRAY);
    warpPerspective(empty, empty, change, empty.size());
    empty = empty(Rect(472, 52, 328, 778));
    cout << "Queue Density" << '\n';
    bool next = true;
    while (next)
    {
        next = cap2.read(frame);
        if (!next) break;
        framenum++;
        if (framenum > 300) break;
        //resize(frame1, frame, Size(x, y));
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        warpPerspective(frame, frame, change, frame.size());
        frame = frame(Rect(472, 52, 328, 778));
        absdiff(frame, empty, subt);
        threshold(subt, subt, 60, 255, THRESH_BINARY);
        dilate(subt, subt, getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2)));
        String name = "Traffic", name1 = "Queue";
        namedWindow(name, WINDOW_NORMAL);
        namedWindow(name1, WINDOW_NORMAL);
        imshow(name, frame);
        imshow(name1, subt);
        int total = subt.total();
        double pixel = (countNonZero(subt) * 1.0) / (total * 1.0);
        error1 += abs(baseline[framenum] - pixel);
        cout << framenum << " " << pixel << endl;
        int press = waitKey(10);
        if (press == 27)
        {
            cout << "Stopping....";
            break;
        }
        if (!next) break;

    }
    cap2.release();
    destroyAllWindows();
    error1 /= framenum;
    return;
}

int main()
{
    cor_fin.push_back(Point2f(472, 52));
    cor_fin.push_back(Point2f(472, 830));
    cor_fin.push_back(Point2f(800, 830));
    cor_fin.push_back(Point2f(800, 52));
    ofstream file;
    fstream file1;
    file1.open("stationary.csv");
    string temp, curline, word;
    while (file1 >> temp)
    {
        reverse(temp.begin(), temp.end());
        while (temp.back() != ',') temp.pop_back();
        temp.pop_back();
        reverse(temp.begin(), temp.end());
        baseline.push_back(stod(temp));
    }
    time_t start, end;
    file.open("queue.csv");
    vector<pair<int, int>>v;
    v.push_back({ 30,30 });
    //v.push_back({ 700,360 });
    v.push_back({ 20,10 });
    for (int x = 1; x <= 2;++x)
    {
        error1 = 0;
        time(&start);
        get(1,1);
        time(&end);
        double dur = double(end - start);
        file << error1 << "," << dur << endl;
    }
    file.close();
    file1.close();
}