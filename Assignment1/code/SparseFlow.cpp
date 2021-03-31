#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
using namespace cv;
using namespace std;
vector<Point2f> cor_init, cor_fin;
double error1 = 0;
vector<double> baseline;
ofstream file;
ofstream file1;
void Click(int event, int x, int y, int flags, void *userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		cor_init.push_back(Point2f(x, y));
	}
}

void Sparse(string pq)
{
	vector<Scalar> colors;
	RNG rng;
	for (int i = 0; i < 100; i++)
	{
		int r = rng.uniform(0, 256);
		int g = rng.uniform(0, 256);
		int b = rng.uniform(0, 256);
		colors.push_back(Scalar(r, g, b));
	}

	VideoCapture cap2(pq);
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
	while (getline(read, u))
	{
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
	cout << "Dynamic Density using Sparse Optical Flow" << '\n';
	bool next = true;
	next = cap2.read(frame);
	framenum++;
	Mat temp = frame;
	double last1 = -1, last2 = -1;
	vector<Point2f> p0, p1;
	cvtColor(temp, temp, COLOR_BGR2GRAY);
	warpPerspective(temp, temp, change, temp.size());
	temp = temp(Rect(472, 52, 328, 778));
	Mat mask = Mat::zeros(temp.size(), temp.type());
	fstream e("dense.csv");
	string erf;
	while (next)
	{
		bool next = cap2.read(frame);
		if (!next) break;
		framenum++;
		cvtColor(frame, frame, COLOR_BGR2GRAY);
		warpPerspective(frame, frame, change, frame.size());
		frame = frame(Rect(472, 52, 328, 778));
		vector<uchar> status;
		vector<float> err;
		goodFeaturesToTrack(temp, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
		TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
		calcOpticalFlowPyrLK(temp, frame, p0, p1, status, err, Size(15, 15), 2, criteria);
		Mat dist;
		int total = 0;
		for (int i = 0; i < p1.size(); ++i)
		{
			if (norm(p1[i] - p0[i]) > 6.0)
			{
				total++;
			}
		}

		//threshold(bgr, bgr, 30, 255, THRESH_BINARY);
		//dilate(bgr, bgr, getStructuringElement(MORPH_RECT, Size(15, 15), Point(7, 7)));
		//  String name = "Traffic", name1 = "Queue";
		//  namedWindow(name, WINDOW_NORMAL);
		//  imshow(name, frame);
		int ftotal = frame.total();
		double pixel = (1800.0 *total) / (ftotal *1.0);
		double pixel1 = pixel;
		if (last2 > -1)
		{
			pixel += last1;
			pixel += last2;
			pixel /= 3;
		}

		if (getline(e, erf))
		{
			int index = erf.find(',');
			double val = stod(erf.substr(index + 1, erf.size() - 1 - index));
			error1 += pow(val - pixel, 2);
			//   cout << error1 << '\n';
		}

		last2 = last1;
		last1 = pixel1;
		temp = frame;
		if (temp.empty())
			break;
	}

	cap2.release();
	destroyAllWindows();
	error1 /= framenum;
	return;
}

int main(int argc, char **argv)
{
	cor_fin.push_back(Point2f(472, 52));
	cor_fin.push_back(Point2f(472, 830));
	cor_fin.push_back(Point2f(800, 830));
	cor_fin.push_back(Point2f(800, 52));
	file.open("extra.txt", std::ios_base::app);
	auto start = Clock::now();
	if (argc == 1)
	{
		Sparse("vid.mp4");
	}
	else
	{
		Sparse(argv[1]);
	}

	auto end = Clock::now();
	auto dur = std::chrono::duration_cast<std::chrono::nanoseconds > (end - start).count() / pow(10, 9);
	cout << error1 << '\n';
	file << "Sparse Error: " << error1 << '\n';
	file << "Sparse Runtime: " << dur << '\n';
	file.close();
}
