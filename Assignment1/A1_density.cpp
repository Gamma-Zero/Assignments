#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

vector<Point2f>cor_init, cor_fin;
void Click(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        cor_init.push_back(Point2f(x, y));
    }
}

int main()
{
    VideoCapture cap("vid.mp4");
    if (!cap.isOpened())
    {
        cout << "Error, cannot open the video.";
        cin.get();
        return 0;
    }
    double fps = cap.get(CAP_PROP_FPS);
    cout << "Frames per seconds : " << fps << endl;
    Ptr<BackgroundSubtractor> bgs = createBackgroundSubtractorMOG2();
    Mat frame, subt, img;
    cap >> img;

    cvtColor(img, img, COLOR_BGR2GRAY);
    namedWindow("Display", WINDOW_NORMAL);
    setMouseCallback("Display", Click, NULL);
    imshow("Display", img);
    waitKey(0);
    cor_fin.push_back(Point2f(472, 52));
    cor_fin.push_back(Point2f(472, 830));
    cor_fin.push_back(Point2f(800, 830));
    cor_fin.push_back(Point2f(800, 52));
    Mat change = findHomography(cor_init, cor_fin);
    Mat crop, view;
    while (true)
    {
        bool next = cap.read(frame);
        if (!next) break;
        next = cap.read(frame);
        if (!next) break;
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        warpPerspective(frame, frame, change, frame.size());
        frame = frame(Rect(472, 52, 328, 778));
        bgs->apply(frame, subt, 0);
        String name = "Traffic", name1 = "Queue";
        namedWindow(name, WINDOW_NORMAL);
        namedWindow(name1, WINDOW_NORMAL);
        imshow(name, frame);
        imshow(name1, subt);
        int press = waitKey(10);
        if (press == 27)
        {
            cout << "Stopping....";
            break;
        }
        next = cap.read(frame);
        if (!next) break;

    }
    cap.release();

}