#pragma once
#ifndef	EDGEDETECTION_H
#define EDGEDETECTION_H

#include "lib.h"

class EdgeDetector
{
private:
public:
	int xGradient_Sobel(Mat img, int x, int y);
	int yGradient_Sobel(Mat img, int x, int y);

	int xGradient_Prewitt(Mat img, int x, int y);
	int yGradient_Prewitt(Mat img, int x, int y);

	int detectBySobel(const Mat& sourceImage, Mat& destinationImage);

	int detectByPrewitt(const Mat& sourceImage, Mat& destinationImage);

	int detectByLaplace(const Mat& sourceImage, Mat& destinationImage);

	int detectByCanny(const Mat& sourceImage, Mat& destinationImage, int low, int high);

	EdgeDetector();
	~EdgeDetector();
};


#endif