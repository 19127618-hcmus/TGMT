#include "lib.h"
#include "LocalFeatures.h"
int main()
{
	Mat trainImg1 = imread("./img/training_images/01_1.jpg", IMREAD_COLOR);
	Mat trainImg2 = imread("./img/training_images/01_2.jpg", IMREAD_COLOR);
	Mat trainImg3 = imread("./img/training_images/01_3.jpg", IMREAD_COLOR);

	Mat testImg = imread("./img/TestImages/01.jpg", IMREAD_COLOR);

	Mat des;

	// imshow("test", testImg);
	// waitKey(0);

	Hariss harrisImg;
	harrisImg.detectHarris(testImg, des);


	return 0;
}