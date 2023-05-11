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

bool isInside(Mat img, Point p) {
	int height = img.rows;
	int width = img.cols;
	if (p.y <= height && p.x <= width && p.y >= 0 && p.x >= 0)
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

Mat_<uchar> dilatation(Mat_<uchar> src, std::vector<Point2i> EL, uchar object)
{
	uchar background = object == 0 ? 255 : 0;
	uchar val;
	int height = src.rows;
	int width = src.cols;
	Mat_<uchar> dst(height, width, background);
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			Point2i point(j, i);
			val = src(point);
			if (val == object) {
				for (int k = 0; k < EL.size(); k++)
				{
					Point2i n = point + EL[k];
					dst(n) = object;
				}
			}
		}
	}
	return dst;
}
void dilatationImg() {
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		int object;
		/*cout << "Object color value : ";
		cin >> object;*/
		Mat_<uchar> dst = dilatation(src, N4, 0);

		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}
Mat_<uchar> erosion(Mat_<uchar> src, std::vector<Point2i> EL, uchar object)
{
	uchar background = object == 0 ? 255 : 0;
	uchar val;
	int height = src.rows;
	int width = src.cols;
	Mat_<uchar> dst(height, width, background);
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			Point2i point(j, i);
			val = src(point);
			if (val == object) {
				bool ok = true;
				for (int k = 0; k < EL.size(); k++)
				{
					Point2i n = point + EL[k];
					if (src(n) != object)
					{
						ok = false;
						break;
					}
				}
				if (ok)
					dst(point) = object;
			}
		}
	}
	return dst;
}
void erosionImg() {
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		int object;
		/*cout << "Object color value : ";
				cin >> object;*/
		Mat_<uchar> dst = erosion(src, N4, 0);
		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}
Mat_<uchar> opening(Mat_<uchar> src, std::vector<Point2i> EL, uchar object)
{
	uchar background = object == 0 ? 255 : 0;
	uchar val;
	int height = src.rows;
	int width = src.cols;
	Mat_<uchar> dst(height, width, background);
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			Point2i point(j, i);
			val = src(point);
			if (val == object) {
				bool ok = true;
				for (int k = 0; k < EL.size(); k++)
				{
					Point2i n = point + EL[k];
					if (src(n) != object)
					{
						ok = false;
						break;
					}
				}
				if (ok)
					dst(point) = object;
			}
		}
	}
	Mat_<uchar> dst2 = dst.clone();
	for (int i = 1; i < height - 1; i++) {
		for (int j = 1; j < width - 1; j++) {
			Point2i point(j, i);
			val = dst(point);
			if (val == object) {
				for (int k = 0; k < EL.size(); k++)
				{
					Point2i n = point + EL[k];
					dst2(n) = object;
				}
			}
		}
	}
	return dst2;

}
void openingImg() {
	char fname[MAX_PATH];
	while (openFileDlg(fname)) {
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		int object;
		/*cout << "Object color value : ";
						cin >> object;*/
		Mat_<uchar> dst = opening(src, N4, 0);
		imshow("src", src);
		imshow("dst", dst);
		waitKey(0);
	}
}
Mat_<uchar> negative(Mat_<uchar> src)
{
	int height = src.rows;
	int width = src.cols;
	Mat_<uchar> dst(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			dst(i, j) = src(i, j) == 255 ? 0 : 255;
	}
	return dst;
}
Mat_<uchar> sub(Mat_<uchar> src1, Mat_<uchar> src2)
{
	int height = src1.rows;
	int width = src1.cols;
	Mat_<uchar> dst(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dst(i, j) = src1(i, j) - src2(i, j);
			if (dst(i, j) < 0)
				dst(i, j) = 0;

		}

	}
	return dst;
}
Mat_<uchar> bite_or(Mat_<uchar> src1, Mat_<uchar> src2)
{
	int height = src1.rows;
	int width = src1.cols;
	Mat_<uchar> dst(height, width);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dst(i, j) = src1(i, j) | src2(i, j);
		}
	}
	return dst;
}
bool empty(Mat_<uchar> src)
{
	int height = src.rows;
	int width = src.cols;
	bool ok = true;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			if (src(i, j) != 0)
				ok = false;
	}
	return ok;
}
void skeleton() {

	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		int width = src.cols;
		int height = src.rows;
		Mat_<uchar> skel = Mat_<uchar>::zeros(height, width);
		cv::Mat temp;
		cv::Mat eroded;
		src = negative(src);
		bool done;
		do
		{
			eroded = erosion(src, N4, 255);
			temp = dilatation(eroded, N4, 255);
			temp = sub(src, temp);
			skel = skel | temp;
			eroded.copyTo(src);
			done = empty(src);// (cv::countNonZero(src) == 0);
		} while (!done);

		imshow("skeleton", skel);
		imshow("src", src);
		waitKey(0);
	}
}
void labeling()
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
						for (int k = 0; k < N4.size(); k++)
						{
							Point2i n = point + N4[k];
							if (isInside(src, n)) {
								uchar neighborValue = src(n);
								uchar neighborLabel = labels(n);
								if (neighborLabel == 0 && neighborValue == 0)
								{
									labels(n) = label;
									Q.push(n);
								}
							}
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
void cropIMG()
{
	char fileName[MAX_PATH];
	while (openFileDlg(fileName))
	{
		Mat_<uchar> src = imread(fileName, IMREAD_GRAYSCALE);
		Rect crop = Rect(src.cols / 4, src.rows / 4, src.cols / 2, src.rows / 2);
		std::vector<cv::Point> shape = std::vector<cv::Point>();
		Mat_<uchar> cropImg = src(crop);
		imshow("Input", src);
		imshow("Crop", cropImg);
		waitKey();
	}
}
void contour()
{
	char fileName[MAX_PATH];
	while (openFileDlg(fileName))
	{
		Mat_<uchar> src = imread(fileName, IMREAD_GRAYSCALE);
		Mat_<uchar> dst = dilatation(src, N4, 0);
		dst = sub(src, dst);
		imshow("Input", src);
		imshow("Contour", dst);
		waitKey();
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
			labeling(); break;
		case 2:
			dilatationImg(); break;
		case 3:
			erosionImg(); break;
		case 4:
			openingImg(); break;
		case 5:
			skeleton(); break;
		case 6:
			cropIMG(); break;
		case 7:
			contour(); break;
		}
	} while (op != 0);
	return 0;
}