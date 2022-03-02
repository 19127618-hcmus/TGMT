#include "edgeDetection.h"


int main(int argc, char* argv[])
{
	//program.exe -sob input output
	EdgeDetector edgeDetect;
	Mat blurSrc;
	Mat graySrc;
	Mat des;

	bool success = 0;
	if (argc == 4)
	{
		Mat src = imread(argv[2], cv::IMREAD_COLOR);
		if (!src.data)
		{
			cout << "Wrong path!";
			return 0;
		}
		GaussianBlur(src, blurSrc, Size(3, 3), 0);

		cvtColor(blurSrc, graySrc, COLOR_BGR2GRAY);

		if (compare(argv[1], "-sob"))
		{
			success = edgeDetect.detectBySobel(graySrc, des);
			if (success)
			{
				imshow("Source image", src);
				imshow("Sobel", des);
				imwrite(argv[3], des);
			}
			else cout << "\nNot success!\n";
		}
		else if (compare(argv[1], "-pre"))
		{
			success = edgeDetect.detectByPrewitt(graySrc, des);
			if (success)
			{
				imshow("Source image", src);
				imshow("Prewitt", des);
				imwrite(argv[3], des);
			}
			else cout << "\nNot success!\n";
		}
		else if (compare(argv[1], "-lap"))
		{
			success = edgeDetect.detectByLaplace(graySrc, des);
			if (success)
			{
				imshow("Source image", src);
				imshow("Laplace", des);
				imwrite(argv[3], des);
			}
			else cout << "\nNot success!\n";
		}
		else
		{
			cout << "Wrong function\n";
			return 0;
		}
	}

	//program.exe -can low high input output
	else if (argc == 6)
	{
		Mat src = imread(argv[4], cv::IMREAD_COLOR);
		if (!src.data)
		{
			cout << "Wrong path!";
			return 0;
		}
		GaussianBlur(src, blurSrc, Size(3, 3), 0);

		cvtColor(blurSrc, graySrc, COLOR_BGR2GRAY);

		if (compare(argv[1], "-can"))
		{
			success = edgeDetect.detectByCanny(graySrc, des, char2double(argv, 2), char2double(argv, 3));
			if (success)
			{
				imshow("Source image", src);
				imshow("Canny", des);
				imwrite(argv[5], des);
			}
			else cout << "\nNot success!\n";
		}
		else
		{
			cout << "Wrong function\n";
			return 0;
		}
	}
	else
	{
		cout << "Input error!\n";
		return 0;
	}

	waitKey(0);
	return 0;
}
