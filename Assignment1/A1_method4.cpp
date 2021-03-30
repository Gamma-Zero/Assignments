#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "pthread.h"
using namespace cv;
using namespace std;
int numthreads;
vector<Point2f>cor_init, cor_fin;
Mat img, temp, change, crop, view, empty;
ofstream file;

struct pass {
	Mat frame;
	long long fn;
	float ans;
};

void* process(void* arg)
{
	Mat subt;
	struct pass *p=(struct pass*) arg;
	Mat frame=p->frame;
	long long framenum=p->fn;
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        warpPerspective(frame, frame, change, frame.size());
        frame = frame(Rect(472, 52, 328, 778));
        absdiff(frame, empty, subt);
        threshold(subt, subt, 60, 255, THRESH_BINARY);
        dilate(subt, subt, getStructuringElement(MORPH_RECT, Size(5, 5), Point(2, 2)));
        int total = subt.total();
        p->ans = (countNonZero(subt) * 1.0) / (total * 1.0);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	fstream file("m4.csv");
	for (numthreads=1;numthreads<=8;numthreads++){
	VideoCapture cap2("vid.mp4");
	if (!cap2.isOpened())
    {
        cout << "Error, cannot open the video.";
        cin.get();
        return 0;
    }
	if (numthreads==1){
		cout << "Method 4" << '\n';
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
    change = findHomography(cor_init, cor_fin);
    empty = imread("empty2.png");
    cvtColor(empty, empty, COLOR_BGR2GRAY);
    warpPerspective(empty, empty, change, empty.size());
    empty = empty(Rect(472, 52, 328, 778));
	}
	cout << "Number of Threads: " << numthreads << '\n';
	fstream e("stationary.csv");
    string erf;
    bool next=1;
    pthread_t threads[numthreads];
    Mat frame;
    long long framenum=0;
    struct pass temp[numthreads];
    clock_t start,end;
    float error=0;
    start=clock();
    while (next)
    {
	    int done=0;
	    for(int i=0;i<numthreads;++i){
            	next = cap2.read(frame);
		if (!next){
			break;
		}
		done++;
            	framenum++;
		temp[i].frame=frame;
		temp[i].fn=framenum;
		pthread_create(&threads[i],NULL,process,&temp[i]);
	    }
	    for(int i=0;i<done;++i){
	    	pthread_join(threads[i],NULL);
		if (getline(e,erf)){
                	error+=pow((temp[i].ans-stof(erf)),2);
            	}
	    }
    }
    cap2.release();
    cv::destroyAllWindows();
    end=clock();
    cout << sqrt(error)/(framenum*1.0) << " " << float(end-start)/float(CLOCKS_PER_SEC) << setprecision(5) << '\n';
    file << sqrt(error)/(framenum*1.0) << " " << float(end-start)/float(CLOCKS_PER_SEC) << setprecision(5) << '\n';
	}
	file.close();
}
