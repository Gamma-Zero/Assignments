#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
using namespace cv;
using namespace std;

vector<Point2f>cor_init, cor_fin;
void get(int x)
{
    VideoCapture cap2("vid.mp4");
    if (!cap2.isOpened())
    {
        cout << "Error, cannot open the video.";
        cin.get();
        return ;
    }
    double fps = cap2.get(CAP_PROP_FPS);
    Mat frame, subt, img, temp;
    cap2 >> img;
    cvtColor(img, img, COLOR_BGR2GRAY);
    fstream read("cor.csv");
    string u;
    while (getline(read,u)){
	    int index=u.find(",");
	    cor_init.push_back(Point2f(stoi(u.substr(0,index)),stoi(u.substr(index+1,u.size()-1-index))));
    }
    read.close();
    cor_fin.push_back(Point2f(472, 52));
    cor_fin.push_back(Point2f(472, 830));
    cor_fin.push_back(Point2f(800, 830));
    cor_fin.push_back(Point2f(800, 52));
    Mat change = findHomography(cor_init, cor_fin);
    Mat crop, view;
    long long framenum = 0;
    ofstream file;
    file.open("stationary.csv");
    Mat empty = imread("empty2.png");
    cvtColor(empty, empty, COLOR_BGR2GRAY);
    warpPerspective(empty, empty, change, empty.size());
    empty = empty(Rect(472, 52, 328, 778));
    cap2.read(frame);
    bool next=1;
    cout << "Generating Baseline..." << '\n';
    while (next)
    {
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        warpPerspective(frame, frame, change, frame.size());
        frame = frame(Rect(472, 52, 328, 778));
        absdiff(frame, empty, subt);
        threshold(subt, subt, 60, 255, THRESH_BINARY);
        dilate(subt, subt, getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2)));
   /*    namedWindow(name, WINDOW_NORMAL);
        namedWindow(name1, WINDOW_NORMAL);
        imshow(name, frame);
        imshow(name1, subt); */
        int total = subt.total(); 
        file << (countNonZero(subt) * 1.0) / (total * 1.0) << endl;
/*        cout << framenum << " " << (countNonZero(subt) * 1.0) / (total * 1.0) << endl;
	int press = waitKey(10);
        if (press == 27)
        {
            cout << "Stopping....";
            break;
        } */
        for (int ss = 0; ss < x ; ++ss)
        {
            next = cap2.read(frame);
            framenum++;
        }

    }
    file.close();
    cap2.release();
    cv::destroyAllWindows();
    return ;
}

int main()
{
    get(1);
}
