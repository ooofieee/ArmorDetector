// TrackingTest.cpp: 定义应用程序的入口点。
//

#include <iostream>
#include<opencv2/highgui.hpp>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include"findLghtBar.h"
#include"findNum.h"

using namespace std;
using namespace cv;

void MYshow(string imgname, Mat img)
{
	Mat imgResized;
	resize(img, imgResized, Size(), 0.75, 0.75);
	cv::imshow(imgname, imgResized);
}


void findHSV(VideoCapture video)
{
	int hmin = 0, smin = 0, vmin = 0;
	int hmax = 179, smax = 255, vmax = 255;
	namedWindow("HSVtrackbars", (640, 2560));
	createTrackbar("hmin", "HSVtrackbars", &hmin, 179);
	createTrackbar("hmax", "HSVtrackbars", &hmax, 179);
	createTrackbar("smin", "HSVtrackbars", &smin, 255);
	createTrackbar("smax", "HSVtrackbars", &smax, 255);
	createTrackbar("vmin", "HSVtrackbars", &vmin, 255);
	createTrackbar("vmax", "HSVtrackbars", &vmax, 255);

	Mat cam0, camHSV, mask;

	while (true)
	{
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		video >> cam0;
		cvtColor(cam0, camHSV, COLOR_BGR2HSV);
		inRange(camHSV, lower, upper, mask);
		MYshow("mask", mask);
		MYshow("cam", cam0);
		cv::waitKey(int(float(1 / video.get(5)) * 1000));
	}
}



int main()
{
	string pathNumTest = "C:/Users/ofine/source/repos/TrackingTest/TrackingTest/src/resources/numTest-dark.png";
	string path1 = "C:/Users/ofine/source/repos/TrackingTest/TrackingTest/src/resources/test1.avi";
	string path2 = "../resources/test2.mp4";
	VideoCapture video1(path1);
	VideoCapture video2(path2);
	Mat numUnchecked = imread(pathNumTest);

	//findHSV(video1);
	//findLightBar(video1);
	findNum(numUnchecked);

	return 0;
}
