#include "LocalFeatures.h"

Hariss::Hariss()
{
	k = 0.04;
	gaussKernel = Size(3,3);
	gaussSigma = 1;

	NMSKernel = 3;
	NMSThreshold = 0.02;
}
Hariss::~Hariss()
{}

int Hariss::detectHarris(const Mat& srcImg, Mat& desImg)
{
	//blur
	Mat blurImg;
	GaussianBlur(srcImg, blurImg, gaussKernel, gaussSigma);
	//convert srcImg to gray img
	Mat grayImg;
	cvtColor(blurImg, grayImg, COLOR_BGR2GRAY);

	Mat gx, gy, gxAbs, gyAbs, gradient;
	Sobel(grayImg, gx, CV_16F, 1, 0, 3);
	Sobel(grayImg, gy, CV_16F, 0, 1, 3);

	convertScaleAbs(gx, gxAbs);
	convertScaleAbs(gy, gyAbs);

	addWeighted(gxAbs, 0.5, gyAbs, 0.5, 0, gradient);

	imshow("m", gradient);
	waitKey(0);

	return 1;
}
