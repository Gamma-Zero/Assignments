#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
vector<Point2f>cor_init,cor_fin;
void Click(int event, int x, int y, int flags, void* userdata)
{
		if (event == EVENT_LBUTTONDOWN)
		{
			cor_init.push_back(Point2f(x, y));
		}
}

void solve(Mat img)
{
	if (img.empty())
		cout << "Image is empty, cannot load image" << endl;
	else
	{
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
			return;
		}
		cor_fin.push_back(Point2f(472, 52));
		cor_fin.push_back(Point2f(472, 830));
		cor_fin.push_back(Point2f(800, 830));
		cor_fin.push_back(Point2f(800, 52));
		Mat change = findHomography(cor_init, cor_fin);
		Mat img_fin;
		warpPerspective(img, img_fin, change, img.size());
		Mat img_cropped = img_fin(Rect(472, 52, 328, 778));
		namedWindow("Transformed Image", WINDOW_NORMAL);
		namedWindow("Cropped Image", WINDOW_NORMAL);
		imshow("Transformed Image", img_fin);
		imshow("Cropped Image", img_cropped);
		imwrite("Transformed.jpg", img_fin);
		imwrite("Cropped.jpg", img_cropped);
		waitKey(0);

	}
}

int main(int argc, char** argv)
{
	Mat img;
        if (argc==1){
                img=imread("empty.jpg");
        } else {
                img = imread(argv[1]);
        }
	solve(img);
}
