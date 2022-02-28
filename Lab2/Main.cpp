#include "edgeDetection.h"

int main()
{
	Mat src = imread("./img/3.jpg", IMREAD_COLOR);

	// imshow("img1", src);
	// waitKey(0);

	EdgeDetector edgeDetect;

	Mat blurSrc;
	Mat graySrc;
	Mat graySrc2;
	Mat des;
	Mat des2;

	GaussianBlur(src, blurSrc, Size(3, 3), 0);

	cvtColor(blurSrc, graySrc, COLOR_BGR2GRAY);
	cvtColor(src, graySrc2, COLOR_BGR2GRAY);

	edgeDetect.detectByCany(graySrc, des);
	// edgeDetect.detectByCany(graySrc2, des2);

	imshow("Sobel", des);
	// imshow("Sobel-noBlur", des2);
	waitKey(0);
	
	return 0;
}