#include"findLghtBar.h"

void findLightBar(VideoCapture video)
{
	int mode = 0;

	Mat frame, frameHSV, maskBlur, maskHSV, maskCanny, maskDilHSV, dst;
	vector<vector<int>> LightBarHSV = { { 0,179,0,255,250,255 } ,{0,3,0,28,234,255} };
	int hmin = LightBarHSV[mode][0], smin = LightBarHSV[mode][2], vmin = LightBarHSV[mode][4];
	int hmax = LightBarHSV[mode][1], smax = LightBarHSV[mode][3], vmax = LightBarHSV[mode][5];
	Scalar lowerHSV(hmin, smin, vmin);
	Scalar upperHSV(hmax, smax, vmax);
	Mat kernelHSV = getStructuringElement(MORPH_RECT, Size(9, 9));



	while (1)
	{
		video >> frame;

		if (frame.empty())
		{
			cout << "video done" << endl;
			break;
		}

		cvtColor(frame, frameHSV, COLOR_BGR2HSV);
		inRange(frameHSV, lowerHSV, upperHSV, maskHSV);
		GaussianBlur(maskHSV, maskBlur, Size(7, 7), 0);
		Canny(maskHSV, maskCanny, 25, 75, 3);
		dilate(maskCanny, maskDilHSV, kernelHSV);

		vector<vector<Point>> contoursHSV;
		vector<Vec4i> hierarchy;



		findContours(maskDilHSV, contoursHSV, hierarchy,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		vector<RotatedRect> boundRect(contoursHSV.size());
		int noise = 0;
		for (int i = 0; i < contoursHSV.size(); i++)
		{
			/*drawContours(frame, contoursHSV, i, Scalar(255,255,255), 2);*/

			if (contoursHSV[i].size() >= 6)
			{
				boundRect[i] = fitEllipse(contoursHSV[i]);
			}
			else
			{
				noise++;
			}
		}

		Point2f topLeft, bottomRight,center;
		double gap, angle, averageHeight;


		for (int i = 0; i < boundRect.size() - noise; i++)
		{
			double area_i = contourArea(contoursHSV[i]);
			if (area_i > 400)
			{
				for (int j = i + 1; j < boundRect.size() - noise; j++)
				{
					double area_j = contourArea(contoursHSV[j]);
					if (area_j > 400)
					{
						averageHeight = (boundRect[i].size.height + boundRect[j].size.height) / 2;
						gap = boundRect[i].center.x - boundRect[j].center.x;
						center = (boundRect[i].center + boundRect[j].center) / 2;
						topLeft = center - Point2f(gap / 2, gap / 2);
						bottomRight = center + Point2f(gap / 2, gap / 2);

						//if (boundRect[i].angle > 88 && boundRect[i].angle < 92 && boundRect[j].angle>88 && boundRect[j].angle < 92)
						//{
						//		goto FLAG;
						//}
							if ((double)(boundRect[i].angle / boundRect[j].angle) < 1.4 && (double)(boundRect[i].angle / boundRect[j].angle) > 0.6)//角度判断
							{
								if (boundRect[i].center.y / boundRect[j].center.y < 1.05 && boundRect[i].center.y / boundRect[j].center.y > 0.95)//中心点y值判断
								{
									if (boundRect[i].size.height / boundRect[j].size.height < 1.2 && boundRect[i].size.height / boundRect[j].size.height > 0.8)//灯条长度判断
									{

										rectangle(frame, topLeft, bottomRight, Scalar(0, 255, 0), 2);

									}
								}
							}
					}
				}
			}
		}



	
				
				
			
		




			cv::imshow("video", frame);
			cv::imshow("videoPreProcessed-HSV", maskHSV);
			//cv::imshow("videoPreProcessed-BGR", maskBGR);
			cv::waitKey(1);
		}


	}

