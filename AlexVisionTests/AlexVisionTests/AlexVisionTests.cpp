// AlexVisionTests.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int hue_min = 0, hue_max = 180, sat_min = 0, sat_max = 25, val_min = 255, val_max = 255;

int min_area = 200, max_area = 10000000;

float aspectRatio = 212.0 / 137;
float screenWidthPx = 640;
float tapeToTape = 8.5;
float zeroDistanceIn = 26.5;
float zeroBoxWidthPx = 199;

float boxWidthPx = 0, boxHeightPx = 0;

float perpDistance = 0, horizDistance = 0, rawDistance = 0;

float pxPerInch = 0;
float screenWidthIn = 0;

float zeroPxPerInch = zeroBoxWidthPx / tapeToTape;
float zeroScreenWidthIn = screenWidthPx/zeroPxPerInch;

float distanceRatio = zeroDistanceIn / zeroScreenWidthIn;

float adjustedWidthPx = aspectRatio * boxHeightPx;

float angle = 3.14/2;

/*
Find the pxPerInch for the view then get the total width of the screen
*/

int main() {

	namedWindow("bars", 1);

	createTrackbar("Hue_Min", "bars", &hue_min, 180);
	createTrackbar("Hue_Max", "bars", &hue_max, 180);
	createTrackbar("Sat_Min", "bars", &sat_min, 255);
	createTrackbar("Sat_Max", "bars", &sat_max, 255);
	createTrackbar("Val_Min", "bars", &val_min, 255);
	createTrackbar("Val_Max", "bars", &val_max, 255);
	
	//Mat imgRgb = imread("color.jpg");

	VideoCapture cap(0);
	if (!cap.isOpened()) return -1;
	
	for (;;) {

		Mat imgRgb;
		cap >> imgRgb;

		Mat imgHsv;
		//cvtColor(imgRgb, imgHsv, CV_BGR2HSV);
		cvtColor(imgRgb, imgHsv, COLOR_BGR2HSV);

		Mat imgGrey;
		inRange(imgHsv, Scalar(hue_min, sat_min, val_min), Scalar(hue_max, sat_max, val_max), imgGrey);

		vector<Point> centers;
		vector<Rect> bounders;
		vector<vector<Point	>> contours;
		vector<Vec4i> hierarchy;
		
		findContours(imgGrey, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		for (int i = 0; i < contours.size(); i++) {
			int area = contourArea(contours[i]);
			//cout << area;
			if (area < max_area && area > min_area) {
				bounders.push_back(boundingRect(contours[i]));
			}
		}

		vector<Rect> pair;

		if (bounders.size() > 1) {
			pair = { bounders[0], bounders[1] };
			float minDistance = 1000000000;

			for (int i = 0; i < bounders.size(); i++) {
				for (int j = 0; j < bounders.size(); j++) {
					if (j != i) {
						if (abs(bounders[i].x - bounders[j].x) < minDistance) {
							pair = { bounders[i], bounders[j] };
						}
					}
				}
			}
		}

		if (pair.size() > 1) {

			centers.push_back(Point(pair[0].x + pair[0].width / 2, pair[0].y + pair[0].height / 2));
			centers.push_back(Point(pair[1].x + pair[1].width / 2, pair[1].y + pair[1].height / 2));
			
			boxWidthPx = abs(centers[0].x-centers[1].x);
			boxHeightPx = abs((pair[0].height+pair[1].height)/2);
			adjustedWidthPx = boxHeightPx * aspectRatio;

			if (abs(boxWidthPx / adjustedWidthPx) < 1) {
				angle = asin(boxWidthPx / adjustedWidthPx);
			}
			else {
				angle = asin(1);
			}
			
			screenWidthIn = screenWidthPx / (boxWidthPx / tapeToTape);
			rawDistance = distanceRatio * screenWidthIn;
			perpDistance = rawDistance * sin(angle);
			horizDistance = rawDistance * cos(angle);				
		}

		Mat imgBox = imgRgb.clone();
		for (int i = 0; i < pair.size(); i++) {
			rectangle(imgBox, pair[i], Scalar(0, 0, 255), 2);
			//circle(imgBox, Point(bounders[i].x + bounders[i].width / 2, bounders[i].y + bounders[i].height / 2), 10, Scalar(0, 0, 255), 2);
		}
		
		if (centers.size() > 1) {
			line(imgBox, centers[0], centers[1], Scalar(0, 0, 255),2);
			//putText(imgBox, to_string(boxWidthPx), Point(centers[0].x + boxWidthPx / 2, centers[0].y-10), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 2));
		}

		cout << "angle " << angle * (180 / 3.1415) << " perp: " << perpDistance << " horiz: " << horizDistance << endl;

		//imshow("image", imgBox);
		//imshow("imgrey", imgGrey);
		if (waitKey(30) == 27) break;
	}

}
