#include"findNum.h"

void findNum(Mat numUnchecked)
{
	string path = "C:/Users/ofine/source/repos/TrackingTest/TrackingTest/src/resources/Num.png";
	Mat num = imread(path);

	//////////标准数字0~9的处理//////////

	Mat numCanny,numDil,numEro;
	Mat num0, num1, num2, num3, num4, num5, num6, num7, num8, num9;
	vector<Mat> numListTemp = { num0, num1, num2, num3, num4, num5, num6, num7, num8, num9 };
	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

	//二值图像获取与分割
	Canny(num, numCanny, 25, 75);
	dilate(numCanny, numDil, kernel);
	erode(numDil, numEro, kernel);
	
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(numEro, contours, hierarchy,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(numEro, contours, i, Scalar(255), -1);
		boundRect[i] = boundingRect(contours[i]);
		Rect roi(boundRect[i].tl(), boundRect[i].br());
		numListTemp[i] = numEro(roi);
	}

	vector<Mat> numList = { numListTemp[5], numListTemp[8], numListTemp[2],numListTemp[3],numListTemp[4],numListTemp[9],numListTemp[6],numListTemp[7],numListTemp[1],numListTemp[0] };
	
	//////////传入图像的处理//////////

	Mat numResized , numPreProcessed,numBlur;

	Canny(numUnchecked, numCanny, 25, 75);
	dilate(numCanny, numDil, kernel);
	erode(numDil, numEro, kernel);

	vector<vector<Point>> contoursTemp;
	findContours(numEro, contoursTemp, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<Rect> boundRectTemp(contours.size());
	vector<Mat> canvasList(numList.size());
	for (int i = 0; i < numList.size(); i++)
	{
		canvasList[i] = Mat::zeros(numList[i].rows, numList[i].cols, CV_8UC1);
	}


	for (int i = 0; i < contoursTemp.size(); i++)
	{
		drawContours(numEro, contoursTemp, i, Scalar(255), -1);
		boundRectTemp[i] = boundingRect(contoursTemp[i]);
		Rect roi(boundRectTemp[i].tl(), boundRectTemp[i].br());
		numUnchecked = numEro(roi);
		
	}

	for (int k = 0; k < numList.size(); k++)
	{
		resize(numUnchecked, numPreProcessed, Size(numList[k].cols, numList[k].rows));
		for (int i = 0; i < numList[k].rows; i++)
		{
			for (int j = 0; j < numList[k].cols; j++)
			{
				if (numPreProcessed.at<uchar>(i, j) == 255 && numList[k].at<uchar>(i, j) == 255)
				{
					
					canvasList[k].at<uchar>(i, j) = 255;

				}

			}
		}
	}

	for (int i = 0; i < numList.size(); i++)
	{
		vector<vector<Point>>contoursCanvas;
		findContours(canvasList[i], contoursCanvas, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		int areaAfter = 0;
		for (int j = 0; j < contoursCanvas.size(); j++)
		{
			areaAfter += contourArea(contoursCanvas[j]);
		}
		vector<vector<Point>> contoursNum;
		findContours(numList[i], contoursNum, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		int areaBefore = 0;
		for (int j = 0; j < contoursNum.size(); j++)
		{
			areaBefore += contourArea(contoursNum[j]);
		}

		cout << abs(areaBefore - areaAfter) << endl;
		if (abs(areaBefore - areaAfter) < 200)
		{
			imshow("numList", num);
			imshow("numOutput", numList[i]);
			imshow("numInput", numPreProcessed);
			waitKey(0);
			
		}
	}



	//imshow("numList", num);
	//imshow("num", numList[1]);
	//imshow("canvas1", canvasList[1]);
	//imshow("numUnchecked", numPreProcessed);
	//waitKey(0);


}