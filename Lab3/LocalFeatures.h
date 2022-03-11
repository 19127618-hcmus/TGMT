#pragma once
#include "lib.h"

class LocalFeature
{
public:
	int detectBlob(const Mat& trainImg, const Mat& testImg, Mat& des);
	int detectDOG(const Mat& trainImg, const Mat& testImg, Mat& des);
	double matchBySIFT(const Mat& trainImg, const Mat& testImg, Mat& des);

};

class Hariss
{
public:
	float k;
	Size gaussKernel;
	float gaussSigma;
	int NMSKernel;
	float NMSThreshold;

	int detectHarris(const Mat& srcImg, Mat& desImg);
	Hariss();
	~Hariss();
};
