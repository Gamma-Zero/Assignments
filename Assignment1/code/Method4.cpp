#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "pthread.h"
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
using namespace cv;
using namespace std;
int numthreads;
vector<Point2f> cor_init, cor_fin;
Mat img, temp, change, crop, view, empty;

struct pass
{
	Mat frame;
	long long fn;
	float ans;
};

void *process(void *arg)
{
	Mat subt;
	struct pass *p = (struct pass *) arg;
	Mat frame = p->frame;
	long long framenum = p->fn;
	cvtColor(frame, frame, COLOR_BGR2GRAY);
	warpPerspective(frame, frame, change, frame.size());
	frame = frame(Rect(472, 52, 328, 778));
	absdiff(frame, empty, subt);
	threshold(subt, subt, 60, 255, THRESH_BINARY);
	dilate(subt, subt, getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2)));
	int total = subt.total();
	p->ans = (countNonZero(subt) *1.0) / (total *1.0);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	ofstream file("m4.csv");
	vector<int> values={1,2,3,4,5,6,10,12,16,20};
	for (auto numthreads:values)
	{
		string z;
		if (argc == 1)
		{
			z = "vid.mp4";
		}
		else
		{
			z = argv[1];
		}
		VideoCapture cap2(z);
		if (!cap2.isOpened())
		{
			cout << "Error, cannot open the video.";
			cin.get();
			return 0;
		}
		if (numthreads == 1)
		{
			cout << "Method 4" << '\n';
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
			change = findHomography(cor_init, cor_fin);
			empty = imread("empty2.png");
			cvtColor(empty, empty, COLOR_BGR2GRAY);
			warpPerspective(empty, empty, change, empty.size());
			empty = empty(Rect(472, 52, 328, 778));
		}
		cout << "Number of Threads: " << numthreads << '\n';
		fstream e("stationary.csv");
		string erf;
		bool next = 1;
		pthread_t threads[numthreads];
		Mat frame;
		long long framenum = 0;
		struct pass temp[numthreads];
		float error = 0;
		auto start = Clock::now();
		while (next)
		{
			int done = 0;
			for (int i = 0; i < numthreads; ++i)
			{
				next = cap2.read(frame);
				if (!next)
				{
					break;
				}
				done++;
				framenum++;
				temp[i].frame = frame;
				temp[i].fn = framenum;
				pthread_create(&threads[i], NULL, process, &temp[i]);
			}
			for (int i = 0; i < done; ++i)
			{
				pthread_join(threads[i], NULL);
				if (getline(e, erf))
				{
					error += pow((temp[i].ans - stof(erf)), 2);
				}
			}
		}
		auto end = Clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::nanoseconds > (end - start).count() / pow(10, 9);
		cout << sqrt(error / framenum *1.0) << " " << dur << setprecision(5) << '\n';
		file << sqrt(error / framenum *1.0) << "," << dur << "," << numthreads << setprecision(5) << '\n';
		cap2.release();
		cv::destroyAllWindows();
	}
	file.close();
}
