#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "pthread.h"
#include <bits/stdc++.h>
#include <chrono>

typedef std::chrono::high_resolution_clock Clock;
using namespace cv;
using namespace std;
int numthreads;
vector<Point2f> cor_init, cor_fin;
Mat img, temp, change, crop, view, empty;
vector<Mat> ep;

struct pass
{
	Mat frame;
	int number;
	long long ans;
};

void *process(void *arg)
{
	struct pass *p = (struct pass *) arg;
	Mat frame = p->frame;
	int i = p->number;
	Mat subt;
	absdiff(frame, ep[i], subt);
	threshold(subt, subt, 60, 255, THRESH_BINARY);
	dilate(subt, subt, getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2)));
	p->ans = countNonZero(subt);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	ofstream file("m3.csv");
	for (numthreads = 1; numthreads <= 8; numthreads++)
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
			cout << "Method 3" << '\n';
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
		int t = 778 / numthreads;
		for (int i = 0; i < numthreads; i++)
		{
			if (i != numthreads - 1)
			{
				Rect myROI(0, i *t, 328, t);
				Mat crop(empty, myROI);
				ep.push_back(crop);
			}
			else
			{
				Rect myROI(0, i *t, 328, 778 - i *t);
				Mat crop(empty, myROI);
				ep.push_back(crop);
			}
		}
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
			next = cap2.read(frame);
			if (!next)
			{
				break;
			}
			framenum++;
			cvtColor(frame, frame, COLOR_BGR2GRAY);
			warpPerspective(frame, frame, change, frame.size());
			frame = frame(Rect(472, 52, 328, 778));
			for (int i = 0; i < numthreads; i++)
			{
				if (i != numthreads - 1)
				{
					temp[i].number = i;
					Rect myROI(0, i *t, 328, t);
					Mat crop(frame, myROI);
					temp[i].frame = crop;
					pthread_create(&threads[i], NULL, process, &temp[i]);
				}
				else
				{
					temp[i].number = i;
					Rect myROI(0, i *t, 328, 778 - i *t);
					Mat crop(frame, myROI);
					temp[i].frame = crop;
					pthread_create(&threads[i], NULL, process, &temp[i]);
				}
			}
			long long total = 0;
			for (int i = 0; i < numthreads; ++i)
			{
				pthread_join(threads[i], NULL);
				total += temp[i].ans;
			}
			float value = (total *1.0) / (frame.total() *1.0);
			if (getline(e, erf))
			{
				error += pow((value - stof(erf)), 2);
			}
			//   cout << framenum << " " << (total*1.0)/(frame.total()*1.0) << '\n';
		}
		auto end = Clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::nanoseconds > (end - start).count() / pow(10, 9);
		cout << sqrt(error / framenum *1.0) << " " << dur << setprecision(5) << '\n';
		file << sqrt(error / framenum *1.0) << "," << dur << setprecision(5) << '\n';
		cap2.release();
		cv::destroyAllWindows();
		ep.clear();
	}
	file.close();
}
