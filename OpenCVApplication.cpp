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
	if (p.x <= height && p.y <= width && p.x >= 0 && p.y >= 0)
		return true;
	else
		return false;
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
			break;
		}
	} while (op != 0);
	return 0;
}