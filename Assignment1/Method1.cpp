#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;
vector<Point2f>cor_init, cor_fin;
double error1 = 0;
vector<double>baseline(1, 0);
void Click(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        cor_init.push_back(Point2f(x, y));
    }
}
void get(int x)
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
        cor_fin.push_back(Point2f(472, 52));
        cor_fin.push_back(Point2f(472, 830));
        cor_fin.push_back(Point2f(800, 830));
        cor_fin.push_back(Point2f(800, 52));
    }
    change = findHomography(cor_init, cor_fin);
    long long framenum = 0;
    Mat empty = imread("empty2.png");
    cvtColor(empty, empty, COLOR_BGR2GRAY);
    warpPerspective(empty, empty, change, empty.size());
    empty = empty(Rect(472, 52, 328, 778));
    cout << "Queue Density" << '\n';
    bool next=true;
    while (next)
    {
        next = cap2.read(frame);
        if (!next) break;
        framenum++;
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
        if (x == 1)
            baseline.push_back(pixel);
        error1 += abs(baseline[framenum] - pixel);
        //file << (framenum * 1.0) / (15.0) << "," << pixel << endl;
        cout << framenum << " " << pixel << endl;
        int press = waitKey(10);
        if (press == 27)
        {
            cout << "Stopping....";
            break;
        }
        for (int ss = 1; ss < x; ++ss)
        {
            next = cap2.read(frame);
            if (!next) break;
            framenum++;
            error1 += abs(pixel-baseline[framenum]);
        }
        if (!next) break;

    }
    cap2.release();
    destroyAllWindows();
    error1 /= framenum;
    return ;
}

int main()
{
    ofstream file;
    time_t start, end;
    file.open("queue.csv");
    for (int x = 1; x <= 1; x += 5)
    {
        error1 = 0;
        time(&start);
        get(1);
        time(&end);
        double dur = double(end - start);
        file << error1 << "," << dur << endl;
    }
    file.close();
}