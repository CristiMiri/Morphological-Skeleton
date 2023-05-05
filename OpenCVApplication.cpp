// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <opencv2/core/utils/logger.hpp>
#include <random>
#include <queue>
#include <stack>
#include <vector>
#include <iostream>

using namespace std;
using namespace cv;
wchar_t* projectPath;


bool isInside(Mat img, int i, int j) {
	int height = img.rows;
	int width = img.cols;
	if (i <= height && j <= width && i >= 0 && j >= 0)
		return true;
	else
		return false;
}

Vec3b* randomColors()
{
	default_random_engine gen;
	uniform_int_distribution<int> d(0, 255);
	uchar x = d(gen);
	Vec3b color[1000];
	for (int i = 0; i < 1000; i++)
		color[i] = Vec3b(d(gen), d(gen), d(gen));
	return color;
}
void coloring(Mat_<uchar> labels) {


	int height = labels.rows;
	int width = labels.cols;
	Mat_<Vec3b> dst(height, width, CV_8UC3);
	Vec3b* colors = randomColors();
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++) {
			if (labels(i, j) == 0)
				dst(i, j) = Vec3b(255, 255, 255);
			else
				dst(i, j) = colors[labels(i, j)];
		}
	}
	imshow("Output", dst);
}
const vector<Point2i> N4 = { Point2i(-1,0),Point2i(0,-1),Point2i(0,1) ,Point2i(1,0) };
const vector<Point2i> N8 = { Point2i(-1,-1),Point2i(-1,0),Point2i(-1,1) ,Point2i(0,-1) ,Point2i(0,1) ,Point2i(1,-1), Point2i(1,0) ,Point2i(1,1) };

void etichetare()
{
	char fileName[MAX_PATH];
	while (openFileDlg(fileName))
	{
		Mat_<uchar> src = imread(fileName, IMREAD_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		int label = 255;

		Mat_<uchar> labels = Mat::zeros(height, width, CV_8UC1);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++) {
				if (src(i, j) == 0 && labels(i, j) == 0)
				{
					label -= 1;
					queue <Point2i> Q;
					labels(i, j) = label;
					Point2i point(j, i);
					Q.push(point);
					while (!Q.empty())
					{
						Point2i point = Q.front();
						Q.pop();
						int pj = point.x;
						int pi = point.y;
						int di[4] = { -1,0,1,0 };
						int dj[4] = { 0,-1,0,1 };
						for (int k = 0; k < N4.size(); k++)
						{
							Point2i n = point + N4[k];
							int ni = n.y;
							int nj = n.x;
							if (isInside(src, ni, nj)) {
								Point2i p(nj, ni);
								uchar neighborValue = src(p);
								uchar neighborLabel = labels(p);
								if (neighborLabel == 0 && neighborValue == 0)
								{
									labels(p) = label;
									Q.push(p);
								}
							}
							/*for (int k = 0; k < 4; k++) {
								int ni = pi + di[k];
								int nj = pj + dj[k];
								if (isInside(src, ni, nj)) {
									Point2i p(nj, ni);
									uchar neighborValue = src(p);
									uchar neighborLabel = labels(p);
									if (neighborLabel == 0 && neighborValue == 0)
									{
										labels(p) = label;
										Q.push(p);
									}
								}
							}*/
						}
					}
				}
			}
		}
		coloring(labels);
		imshow("Input", src);

		waitKey();
		destroyAllWindows();
	}
}
Mat_<uchar> dilatare(Mat_<uchar> src, std::vector<Point2i> EL)
{
	uchar val;
	int height = src.rows;
	int width = src.cols;
	Mat_<uchar> dst(height, width);
	dst = src.clone();
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			Point2i point(j, i);
			val = src(point);
			if (val == 0) {
				for (int k = 0; k < EL.size(); k++)
				{
					Point2i n = point + EL[k];
					dst(n) = 0;
				}
			}
		}
	}
	return dst;
}
void dilatareIMG() {
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		Mat_<uchar> dst = dilatare(src, N4);

		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}
Mat_<uchar> eroziune(Mat_<uchar> src, std::vector<Point2i> EL)
{
	uchar val;
	int height = src.rows;
	int width = src.cols;
	Mat_<uchar> dst(height, width);
	dst = src.clone();
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			Point2i point(j, i);
			val = src(point);
			if (val == 255) {
				for (int k = 0; k < EL.size(); k++)
				{
					Point2i n = point + EL[k];
					dst(n) = 255;
				}
			}
		}
	}
	return dst;
}

Mat_<uchar> eroziune2(Mat_<uchar> src, std::vector<Point2i> EL, uchar pixel)
{
	uchar val;
	int height = src.rows;
	int width = src.cols;
	uchar reverse = pixel == 255 ? 0 : 255;
	Mat_<uchar> dst(height, width, reverse);
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			Point2i point(j, i);
			val = src(point);
			bool inside = false;
			if (val == pixel) {
				for (int k = 0; k < EL.size(); k++)
				{
					Point2i n = point + EL[k];
					if (src(n) == pixel)
						inside = true;
					else {
						inside = false;
						break;
					}
				}
			}
			if (inside == true)
				dst(point) = pixel;
		}
	}
	return dst;
}
void eroziuneIMG() {
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		Mat_<uchar> dst = eroziune(src, N4);
		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}

//write an erode an image until it become completly white
//Mat_<uchar> erodeK(Mat src, int k)
//{
//	Mat_<uchar> dst = src.clone();
//	Mat_<uchar> aux = src.clone();
//	int white = mean(src)[0];
//	while (white != 255)
//	{
//		aux = eroziune(aux, N4);
//		white = mean(aux)[0];
//		if (white == 255)
//		{
//			break;
//		}
//		dst = aux.clone();
//	}
//	return dst;
//}
Mat_<uchar> erodeK(Mat_<uchar> src, int k) {
	Mat_<uchar> dst = src.clone();
	for (int i = 0; i < k; i++) {
		dst = eroziune(dst, N4);
	}
	return dst;
}
int maxErosion(Mat_<uchar> src)
{
	Mat_<uchar> aux = src.clone();
	int k = 0;
	int white = mean(src)[0];
	while (white != 255)
	{
		aux = eroziune(aux, N4);
		white = mean(aux)[0];
		if (white == 255)
		{
			break;
		}
		k++;
	}
	return k;
}
void openImg()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{


		Mat src = imread(fname, IMREAD_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		int white = 0;
		bool empty = countNonZero(src) != 0;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (src.at<uchar>(i, j) == 255)
				{
					white++;
				}

			}
		}
		cout << mean(src)[0] << endl;
		if (white == height * width)
		{
			cout << "Imaginea este complet alba" << endl;
			cout << empty << endl;
		}
		else
		{
			cout << "Imaginea nu este complet alba" << endl;
			cout << empty << endl;
		}


		imshow("input image", src);

		waitKey();
	}
}
Mat_<uchar> deschidere(Mat_<uchar> src, std::vector<Point2i> EL)
{
	uchar val;
	int height = src.rows;
	int width = src.cols;
	Mat_<uchar> dst(height, width);
	Mat_<uchar> dst2(height, width);
	dst = src.clone();
	//eroziune();
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			Point2i point(j, i);
			val = src(point);
			if (val == 255) {
				for (int k = 0; k < EL.size(); k++)
				{
					Point2i n = point + EL[k];
					dst(n) = 255;
				}
			}
		}
	}
	dst2 = dst.clone();
	//dilatare();
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			Point2i point(j, i);
			val = dst(point);
			if (val == 0) {
				for (int k = 0; k < EL.size(); k++)
				{
					Point2i n = point + EL[k];
					dst2(n) = 0;
				}
			}
		}
	}return dst2;
}
void deschidereIMG() {

	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		imshow("src", src);
		Mat_<uchar> dst = deschidere(src, N4);
		imshow("deschidere", dst);
		Mat_<uchar> dst2 = eroziune(src, N4);
		dst2 = dilatare(dst2, N4);
		imshow("deschidere2", dst2);
		waitKey(0);
	}
}
void skeleton()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		Mat_<uchar> eroder = src.clone();
		bool done = true;
		imshow("src", src);
		//Write the code for the skeleton algorithm using eroziuneK and deschidere
		while (done) {
			Mat_<uchar> temp = eroder.clone();
			eroder = deschidere(eroder, N4);
			eroder = src - eroder;
			imshow("skeleton", eroder);
			if (countNonZero(eroder - temp) == 0)
				done = false;
		}
		imshow("src", src);
		imshow("skeleton", eroder);
		waitKey(0);
	}
}
Mat_<uchar> sub(Mat_<uchar> src, Mat_<uchar> src2)
{
	int height = src.rows;
	int width = src.cols;
	Mat_<uchar> dst(height, width);
	uchar val;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
		{
			Point2i point(j, i);
			val = src(point) - src2(point);
			if (val < 0)
			{
				val = 255;
			}
			dst(point) = val;
		}
	}
	return dst;
}
void erodeUntilWhite()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		int width = src.cols;
		int height = src.rows;
		Mat_<uchar> skel(height, width, 255);
		//Mat_<uchar> skel = Mat::zeros(height, width, CV_8UC1);
		Mat_<uchar> temp;
		Mat_<uchar> eroded;
		int white = mean(src)[0];
		//bitwise_not(src, src);
		do
		{

			eroded = eroziune2(src, N4, 255);
			temp = deschidere(src, N4);
			//temp = sub(src, temp);
			cv::subtract(temp, src, temp);
			bitwise_xor(skel, temp, skel);
			eroded.copyTo(src);

			white = mean(src)[0];
		} while (white != 255);

		imshow("skeleton", skel);
		imshow("src", src);
		waitKey(0);
	}
}
void compareErosion()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat_<uchar> img = imread(fname, IMREAD_GRAYSCALE);
		cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
		cv::Mat eroded;
		cv::Mat eroded2;
		cv::erode(img, eroded, element);
		eroded2 = eroziune2(img, N8, 255);
		imshow("erode", eroded);
		imshow("erode2", eroded2);
		imwrite("erode.jpg", eroded);
		imwrite("erode2.jpg", eroded2);
		waitKey(0);
	}

}
void copy()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat_<uchar> img = imread(fname, IMREAD_GRAYSCALE);
		cv::threshold(img, img, 127, 255, cv::THRESH_BINARY);
		cv::Mat skel(img.size(), CV_8UC1, cv::Scalar(0));
		cv::Mat temp;
		cv::Mat eroded;

		cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
		cv::bitwise_not(img, img);
		bool done;
		do
		{
			cv::erode(img, eroded, element);
			//cv::dilate(eroded, temp, element); 
			cv::morphologyEx(img, temp, cv::MORPH_OPEN, element);
			cv::subtract(img, temp, temp);
			cv::bitwise_or(skel, temp, skel);
			eroded.copyTo(img);

			done = (cv::countNonZero(img) == 0);
		} while (!done);
		bitwise_not(skel, skel);
		imshow("src", img);
		imshow("skel", skel);
		waitKey(0);
	}

}
int main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_FATAL);
	projectPath = _wgetcwd(0, 0);

	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Etichetare\n");
		printf(" 2 - Dilatare\n");
		printf(" 3 - Eroziune\n");
		printf(" 4 - Deschidere\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d", &op);
		switch (op)
		{
		case 1:
			etichetare(); break;
		case 2:
			dilatareIMG(); break;
		case 3:
			eroziuneIMG(); break;
		case 4:
			deschidereIMG(); break;
		case 5:
			skeleton(); break;
		case 6:
			copy(); break;
		case 7:
			compareErosion(); break;
		case 8:
			erodeUntilWhite(); break;
		}
	} while (op != 0);
	return 0;
}