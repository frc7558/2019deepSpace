// AlexVisionTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int hue_min = 74, hue_max = 113, sat_min = 0, sat_max = 112, val_min = 213, val_max = 255;

int min_area = 1000, max_area = 10000000;

float constantRatio = 185 / 49;
float center2center = 0;
float linearDistance = 0;

int main() {

	
	namedWindow("bars", 1);

	
	createTrackbar("Hue_Min", "bars", &hue_min, 180);
	createTrackbar("Hue_Max", "bars", &hue_max, 180);
	createTrackbar("Sat_Min", "bars", &sat_min, 255);
	createTrackbar("Sat_Max", "bars", &sat_max, 255);
	createTrackbar("Val_Min", "bars", &val_min, 255);
	createTrackbar("Val_Max", "bars", &val_max, 255);
	
	

	//Mat imgRgb = imread("color.jpg");

	VideoCapture cap(1);
	if (!cap.isOpened()) return -1;
	
	for (;;) {

		Mat imgRgb;
		cap >> imgRgb;

		Mat imgHsv;
		cvtColor(imgRgb, imgHsv, CV_BGR2HSV);

		Mat imgGrey;
		inRange(imgHsv, Scalar(hue_min, sat_min, val_min), Scalar(hue_max, sat_max, val_max), imgGrey);

		Mat imgBlur; 
		GaussianBlur(imgGrey, imgBlur, Size(5, 5), 0, 0);

		Mat canny;
		Canny(imgBlur, canny, 10, 100);

		vector<vector<Point	>> contours;
		vector<Vec4i> hierarchy;

		findContours(canny, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		vector<Rect> bounders;

		for (int i = 0; i < contours.size(); i++) {
			int area = contourArea(contours[i]);
			cout << areal
			if (area < max_area && area > min_area) {
				bounders.push_back(boundingRect(contours[i]));
			}
		}

		if (bounders.size() > 1) {
			center2center = abs((bounders[1].x + bounders[1].width / 2) - (bounders[0].x + bounders[0].width / 2))/2;
			linearDistance = center2center / constantRatio;
		}

		Mat imgBox = imgRgb.clone();
		for (int i = 0; i < bounders.size(); i++) {
			rectangle(imgBox, bounders[i], Scalar(0, 0, 255), 2);
			//circle(imgBox, Point(bounders[i].x + bounders[i].width / 2, bounders[i].y + bounders[i].height / 2), 10, Scalar(0, 0, 255), 2);
		}

		cout << " c2c: " << center2center << " ld: " << linearDistance << endl;

		imshow("image", imgGrey);
		if (waitKey(30) == 27) break;
	}

}
