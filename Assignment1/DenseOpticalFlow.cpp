#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;
vector<Point2f>cor_init, cor_fin;
double error1 = 0;
vector<double>baseline;
ofstream file;
ofstream file1;
void Click(int event, int x, int y, int flags, void* userdata)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        cor_init.push_back(Point2f(x, y));
    }
}
void Dense()
{
    VideoCapture cap2("abc.mp4");
    if (!cap2.isOpened())
    {
        cout << "Error, cannot open the video.";
        cin.get();
        return;
    }
    double fps = cap2.get(CAP_PROP_FPS);
    Mat frame, subt, change;
    fstream read("cor.csv");
    string u;
    while (getline(read, u)) {
        int index = u.find(",");
        cor_init.push_back(Point2f(stoi(u.substr(0, index)), stoi(u.substr(index + 1, u.size() - 1 - index))));
    }
    read.close();
    change = findHomography(cor_init, cor_fin);
    long long framenum = 0;
    Mat empty = imread("empty2.png");
    cvtColor(empty, empty, COLOR_BGR2GRAY);
    warpPerspective(empty, empty, change, empty.size());
    empty = empty(Rect(472, 52, 328, 778));
    cout << "Dynamic Density using Dense Optical Flow" << '\n';
    bool next = true;
    next = cap2.read(frame);
    framenum++;
    Mat temp = frame;
    double last1 = -1, last2 = -1;
    cvtColor(temp, temp, COLOR_BGR2GRAY);
    warpPerspective(temp, temp, change, temp.size());
    temp = temp(Rect(472, 52, 328, 778));
    while (next)
    {
        bool next = cap2.read(frame);
        if (!next) break;
        framenum++;
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        warpPerspective(frame, frame, change, frame.size());
        frame = frame(Rect(472, 52, 328, 778));
        Mat flow(temp.size(), CV_32FC2);
        calcOpticalFlowFarneback(temp, frame, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
        Mat flow_parts[2];
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
        angle *= ((1.0 / 360.0) * (180.0 / 255.0));

        Mat _hsv[3], hsv, hsv8, bgr;
        _hsv[0] = angle;
        _hsv[1] = Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magn_norm;
        merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
        cvtColor(hsv8, bgr, COLOR_HSV2BGR);
        cvtColor(bgr, bgr, COLOR_BGR2GRAY);
        threshold(bgr, bgr, 30, 255, THRESH_BINARY);
        dilate(bgr, bgr, getStructuringElement(MORPH_RECT, Size(15, 15), Point(7, 7)));
        String name = "Traffic", name1 = "Queue";
        namedWindow(name, WINDOW_NORMAL);
        namedWindow(name1, WINDOW_NORMAL);
        imshow(name, frame);
        imshow(name1, bgr);
        int total = bgr.total();
        double pixel = (countNonZero(bgr) * 1.0) / (total * 1.0);
        double pixel1 = pixel;
        if (last2 > -1)
        {
            pixel += last1;
            pixel += last2;
            pixel /= 3;
        }
        cout << framenum << " " << pixel << "\n";
        file << (framenum * 1.0) / (15.0) << "," << pixel << endl;
        int press = waitKey(10);
        if (press == 27)
        {
            cout << "Stopping....";
            break;
        }
        last2 = last1;
        last1 = pixel1;
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
    time_t start, end;
    file.open("dynamic.csv");
    Dense();
    file.close();
}
