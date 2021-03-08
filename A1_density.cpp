#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
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
    //    cout << "Frames per seconds : " << fps << endl;
    //    Ptr<BackgroundSubtractor> bgs = createBackgroundSubtractorMOG2();
    Mat frame, subt, img, temp;
    cap >> img;
    ofstream file;
    file.open("moving.csv");
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
    cout << "Moving Density" << '\n';
    long long framenum = 0;
    while (true)
    {
        bool next = cap.read(frame);
        if (!next) break;
        framenum++;
        temp = frame;
        cvtColor(temp, temp, COLOR_BGR2GRAY);
        warpPerspective(temp, temp, change, temp.size());
        temp = temp(Rect(472, 52, 328, 778));
        next = cap.read(frame);
        if (!next) break;
        framenum++;
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        warpPerspective(frame, frame, change, frame.size());
        frame = frame(Rect(472, 52, 328, 778));
        Mat flow(frame.size(), CV_32FC2);

    // ---- using optical flow

        calcOpticalFlowFarneback(temp, frame, flow, 0.5, 3, 15, 3, 5, 1.2, 0); 
        Mat flow_parts[2];
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
        float a = 180, b = 360 * 255;
        angle *= a / b;
        Mat abc[3], pqr, op, final;
        abc[0] = angle;
        abc[1] = Mat::ones(angle.size(), CV_32F);
        abc[2] = magn_norm;
        merge(abc, 3, pqr);
        pqr.convertTo(op, CV_8U, 255.0);
        cvtColor(op, final, COLOR_HSV2BGR);
        namedWindow("optical_flow", WINDOW_NORMAL);
        imshow("optical_flow", final);

    // --- using difference between adjacent frames

        absdiff(frame, temp, subt);   
        threshold(subt, subt, 63, 255, THRESH_BINARY);
        dilate(subt, subt, getStructuringElement(MORPH_RECT, Size(17, 17), Point(8, 8)));
        String name = "Traffic", name1 = "Queue";
        namedWindow(name, WINDOW_NORMAL);
        namedWindow(name1, WINDOW_NORMAL);
        imshow(name, frame);
        imshow(name1, subt);
        int total = subt.total();
        file << (framenum * 1.0) / (15.0) << "," << (countNonZero(subt) * 1.0) / (total * 1.0) << endl;
        cout << framenum << " " << (countNonZero(subt) * 1.0) / (total * 1.0) << endl;
        int press = waitKey(10);
        if (press == 27)
        {
            cout << "Stopping....";
            break;
        }
        next = cap.read(frame);
        if (!next) break;
        framenum++;

    }
    file.close();
    cap.release();
    destroyAllWindows();
    file.open("stationary.csv");
    Mat empty = imread("empty2.png");
    cvtColor(empty, empty, COLOR_BGR2GRAY);
    warpPerspective(empty, empty, change, empty.size());
    empty = empty(Rect(472, 52, 328, 778));
    VideoCapture cap2("vid.mp4");
    cout << "Stationary Density" << '\n';
    framenum = 0;
    while (true)
    {
        bool next = cap2.read(frame);
        if (!next) break;
        framenum++;
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
        file << (framenum * 1.0) / (15.0) << "," << (countNonZero(subt) * 1.0) / (total * 1.0) << endl;
        cout << framenum << " " << (countNonZero(subt) * 1.0) / (total * 1.0) << endl;
        int press = waitKey(10);
        if (press == 27)
        {
            cout << "Stopping....";
            break;
        }
        next = cap2.read(frame);
        if (!next) break;
        framenum++;

    }
    file.close();
    cap2.release();
    destroyAllWindows();
}
