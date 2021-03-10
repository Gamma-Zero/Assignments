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

int main(int argc, char** argv)
{
    String path;
    if (argc==1){
	    path="vid.mp4";
    } else {
	    path=argv[1];
    }
    VideoCapture cap(path);
    if (!cap.isOpened())
    {
        cout << "Error, cannot open the video.";
        cin.get();
        return 0;
    }
    double fps = cap.get(CAP_PROP_FPS);
//    cout << "Frames per seconds : " << fps << endl;
//    Ptr<BackgroundSubtractor> bgs = createBackgroundSubtractorMOG2();
    vector<long long> f;
    vector<float> qd,dd;
    Mat frame, subt, img, temp;
    cap >> img;
    ofstream file;
    file.open("moving.csv");
    cvtColor(img, img, COLOR_BGR2GRAY);
    namedWindow("Display", WINDOW_NORMAL);
    setMouseCallback("Display", Click, NULL);
    imshow("Display", img);
    waitKey(0);
    while (cor_init.size() > 4)
			cor_init.pop_back();
    if (cor_init.size() < 4)
    {
		cout << "Need atleast 4 points to find Homography" << endl;
		return 0;
    }
    cor_fin.push_back(Point2f(472, 52));
    cor_fin.push_back(Point2f(472, 830));
    cor_fin.push_back(Point2f(800, 830));
    cor_fin.push_back(Point2f(800, 52));
    Mat change = findHomography(cor_init, cor_fin);
    Mat crop, view;
    long long framenum=0;
    while (true)
    {
        bool next = cap.read(frame);
	if (!next) break;
	framenum++;
	temp=frame;
	cvtColor(temp, temp, COLOR_BGR2GRAY);
        warpPerspective(temp, temp, change, temp.size());
        temp = temp(Rect(472, 52, 328, 778));
        next = cap.read(frame);
        if (!next) break;
	framenum++;
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        warpPerspective(frame, frame, change, frame.size());
        frame = frame(Rect(472, 52, 328, 778));
	absdiff(frame,temp,subt);
	threshold(subt,subt,63,255,THRESH_BINARY);
	dilate(subt,subt,getStructuringElement(MORPH_RECT,Size(15,15),Point(7,7)));
        String name = "Traffic", name1 = "Queue";
        namedWindow(name, WINDOW_NORMAL);
        namedWindow(name1, WINDOW_NORMAL);
        imshow(name, frame);
        imshow(name1, subt);
	int total = subt.total();
	file << (framenum*1.0)/(15.0) << "," << (countNonZero(subt)*1.0)/(total*1.0) << endl;
	f.push_back(framenum);
	dd.push_back((countNonZero(subt)*1.0)/(total*1.0));
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
    Mat empty=imread("empty2.png");
    cvtColor(empty, empty, COLOR_BGR2GRAY);
    warpPerspective(empty, empty, change, empty.size());
    empty = empty(Rect(472, 52, 328, 778));
    VideoCapture cap2("vid.mp4");
    framenum=0;
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
        absdiff(frame,empty,subt);
        threshold(subt,subt,63,255,THRESH_BINARY);
        dilate(subt,subt,getStructuringElement(MORPH_RECT,Size(7,7),Point(3,3)));
        String name = "Traffic", name1 = "Queue";
        namedWindow(name, WINDOW_NORMAL);
        namedWindow(name1, WINDOW_NORMAL);
        imshow(name, frame);
        imshow(name1, subt);
        int total = subt.total();
        file << (framenum*1.0)/(15.0) << "," << (countNonZero(subt)*1.0)/(total*1.0) << endl;
	qd.push_back((countNonZero(subt)*1.0)/(total*1.0));
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
    cout << "Framenum" << ", " << "Queue Density" << ", " << "Dynamic Density" << '\n';
    for (int i=0;i<f.size();++i){
	    cout << f[i] << ", " << qd[i] << ", " << dd[i] << '\n';
    }
}
