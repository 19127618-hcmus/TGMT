#include "edgeDetection.h"

int main()
{
	Mat src1 = imread("./img/1.jpg", IMREAD_COLOR);
	Mat src2 = imread("./img/2.jpg", IMREAD_COLOR);
	Mat src3 = imread("./img/3.jpg", IMREAD_COLOR);
	Mat src4 = imread("./img/4.jpg", IMREAD_COLOR);
	Mat src5 = imread("./img/5.jpg", IMREAD_COLOR);

	// imshow("img1", src);
	// waitKey(0);

	EdgeDetector edgeDetect;

	Mat blurSrc;
	Mat graySrc;
	Mat graySrc2;
	Mat des;
	Mat des2;

	/*
	//----
	GaussianBlur(src1, blurSrc, Size(3, 3), 0);

	cvtColor(blurSrc, graySrc, COLOR_BGR2GRAY);

	edgeDetect.detectByLaplace(graySrc, des);
	
	imshow("1-Laplace", des);

	//----
	GaussianBlur(src2, blurSrc, Size(3, 3), 0);

	cvtColor(blurSrc, graySrc, COLOR_BGR2GRAY);

	edgeDetect.detectByLaplace(graySrc, des);

	imshow("2-Laplace", des);

	//----
	GaussianBlur(src3, blurSrc, Size(3, 3), 0);

	cvtColor(blurSrc, graySrc, COLOR_BGR2GRAY);

	edgeDetect.detectByLaplace(graySrc, des);

	imshow("3-Laplace", des);

	//----
	GaussianBlur(src4, blurSrc, Size(3, 3), 0);

	cvtColor(blurSrc, graySrc, COLOR_BGR2GRAY);

	edgeDetect.detectByLaplace(graySrc, des);

	imshow("4-Laplace", des);

	//----
	GaussianBlur(src5, blurSrc, Size(3, 3), 0);

	cvtColor(blurSrc, graySrc, COLOR_BGR2GRAY);

	edgeDetect.detectByLaplace(graySrc, des);

	imshow("5-Laplace", des);
	*/

	//----
	GaussianBlur(src5, blurSrc, Size(3, 3), 0);
	
	cvtColor(blurSrc, graySrc, COLOR_BGR2GRAY);
	
	// edgeDetect.detectByCanny(graySrc, des, 1, 1);

	Canny(graySrc, des2, 10, 100, 3);

	// imshow("1-Canny", des);
	imshow("1-Canny2", des2);

	waitKey(0);
	
	return 0;
}