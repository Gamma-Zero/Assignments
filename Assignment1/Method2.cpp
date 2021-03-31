#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
using namespace cv;
using namespace std;
vector<Point2f> cor_init, cor_fin;
double error1 = 0;
long long framenum;

void get(float x, float y, string z)
{
	VideoCapture cap2(z);
	if (!cap2.isOpened())
	{
		cout << "Error, cannot open the video.";
		cin.get();
		return;
	}

	double fps = cap2.get(CAP_PROP_FPS);
	Mat frame, subt, img, change;
	cout << "Resolution: " << int(x*328) << "X" << int(y*778) << '\n';
	change = findHomography(cor_init, cor_fin);
	framenum = 0;
	Mat empty = imread("empty2.png");
	cvtColor(empty, empty, COLOR_BGR2GRAY);
	warpPerspective(empty, empty, change, empty.size());
	empty = empty(Rect(472, 52, 328, 778));
	resize(empty, empty, Size(),x,y);
	fstream e("stationary.csv");
	string erf;
	bool next = true;
	while (next)
	{
		next = cap2.read(frame);
		if (!next) break;
		framenum++;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		warpPerspective(frame, frame, change, frame.size());
		frame = frame(Rect(472, 52, 328, 778));
		Mat frame1=frame.clone();
		resize(frame1, frame, Size(),x,y);
		absdiff(frame, empty, subt);
		threshold(subt, subt, 60, 255, THRESH_BINARY);
		dilate(subt, subt, getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2)));
		int total = subt.total();
		double pixel = (countNonZero(subt) * 1.0) / (total * 1.0);
		if (getline(e, erf))
		{
			error1 += pow((pixel - stod(erf)), 2);
		}

		if (!next) break;
	}

	cap2.release();
	destroyAllWindows();
	return;
}

int main(int argc, char** argv)
{
	cout << "Method 2" << '\n';
	fstream read("cor.csv");
	string u;
	while (getline(read, u))
	{
		int index = u.find(",");
		cor_init.push_back(Point2f(stoi(u.substr(0, index)), stoi(u.substr(index + 1, u.size() - 1 - index))));
	}

	read.close();
	cor_fin.push_back(Point2f(472, 52));
	cor_fin.push_back(Point2f(472, 830));
	cor_fin.push_back(Point2f(800, 830));
	cor_fin.push_back(Point2f(800, 52));
	ofstream file("m2.csv");
	fstream file1;
	string temp, curline, word;
	vector<pair<float, float>> v;
	v.push_back({ 1.0,1.0 });
	v.push_back({ 0.8,0.8 });
	v.push_back({ 0.7,0.7 });
	v.push_back({ 0.6,0.6 });
	v.push_back({ 0.5,0.5 });
	for (auto i : v)
	{
		error1 = 0;
		auto start = Clock::now();
		if (argc == 1)
		{
			get(i.first, i.second, "vid.mp4");
		}
		else
		{
			get(i.first, i.second, argv[1]);
		}

		auto end = Clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (end - start).count() / pow(10, 9);
		cout << sqrt(error1 / framenum * 1.0) << " " << dur << setprecision(5) << '\n';
		file << sqrt(error1 / framenum * 1.0) << "," << dur << "," << i.first*i.second << setprecision(5) << '\n';
	}

	file.close();
}
