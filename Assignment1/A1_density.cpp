// A1_density.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <opencv2/opencv.hpp>
#include <iostream>
#include "C:/opencv-4.5.1/opencv/build/include/opencv2/videoio.hpp"
using namespace cv;
using namespace std;
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
    String name = "Traffic",name1="Queue";
    namedWindow(name, WINDOW_NORMAL);
    namedWindow(name1, WINDOW_NORMAL);
    Ptr<BackgroundSubtractor> bgs= createBackgroundSubtractorMOG2();
    Mat frame,subt;
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH); 
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    Size frame_size(frame_width, frame_height);
    VideoWriter wr("Small.mp4", VideoWriter::fourcc('M', 'J', 'P', 'G'), 5, frame_size, true);
    while (true)
    {
        bool next = cap.read(frame);
        if (!next)
            break;
        next = cap.read(frame);
        if (!next)
            break;
        next = cap.read(frame);
        if (!next) break;
        bgs->apply(frame, subt);
        wr.write(subt);
        imshow(name, frame);
        imshow(name1, subt);
        int press = waitKey(10);
        if (press == 27)
        {
            cout << "Stopping....";
            break;
        }

    }
    wr.release();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
