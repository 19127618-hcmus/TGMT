#include "edgeDetection.h"

EdgeDetector::EdgeDetector() {}
EdgeDetector::~EdgeDetector() {}

/*---------------------------------------------------------------------------*/
// create kernel

int EdgeDetector::xGradient_Sobel(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1)
		+ 2 * image.at<uchar>(y, x - 1)
		+ image.at<uchar>(y + 1, x - 1)
		- image.at<uchar>(y - 1, x + 1)
		- 2 * image.at<uchar>(y, x + 1)
		- image.at<uchar>(y + 1, x + 1);
}
int EdgeDetector::yGradient_Sobel(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1)
		+ 2 * image.at<uchar>(y - 1, x)
		+ image.at<uchar>(y - 1, x + 1)
		- image.at<uchar>(y + 1, x - 1)
		- 2 * image.at<uchar>(y + 1, x)
		- image.at<uchar>(y + 1, x + 1);
}
int EdgeDetector::xGradient_Prewitt(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1)
		+ image.at<uchar>(y, x - 1)
		+ image.at<uchar>(y + 1, x - 1)
		- image.at<uchar>(y - 1, x + 1)
		- image.at<uchar>(y, x + 1)
		- image.at<uchar>(y + 1, x + 1);
}
int EdgeDetector::yGradient_Prewitt(Mat image, int x, int y)
{
	return image.at<uchar>(y - 1, x - 1)
		+ image.at<uchar>(y - 1, x)
		+ image.at<uchar>(y - 1, x + 1)
		- image.at<uchar>(y + 1, x - 1)
		- image.at<uchar>(y + 1, x)
		- image.at<uchar>(y + 1, x + 1);
}

/*---------------------------------------------------------------------------*/
// Sobel

int EdgeDetector::detectBySobel(const Mat& sourceImage, Mat& destinationImage)
{
	if (!sourceImage.data) return 0;

	int gx = 0, gy = 0, sum = 0;
	Mat output = sourceImage.clone();
	int width = sourceImage.cols, height = sourceImage.rows;

	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			gx = xGradient_Sobel(sourceImage, x, y);
			gy = yGradient_Sobel(sourceImage, x, y);
			
			sum = abs(gx) + abs(gy);
			sum = sum > 255 ? 255 : sum;
			sum = sum < 0 ? 0 : sum;
			output.at<uchar>(y, x) = saturate_cast<uchar>(sum);
		}
	}
	destinationImage = output.clone();
	return 1;
}

/*---------------------------------------------------------------------------*/
//Prewitt

int EdgeDetector::detectByPrewitt(const Mat& sourceImage, Mat& destinationImage)
{
	if (!sourceImage.data) return 0;

	int gx = 0, gy = 0, sum = 0;
	Mat output = sourceImage.clone();
	int width = sourceImage.cols, height = sourceImage.rows;

	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			gx = xGradient_Prewitt(sourceImage, x, y);
			gy = yGradient_Prewitt(sourceImage, x, y);
		
			sum = abs(gx) + abs(gy);
			sum = sum > 255 ? 255 : sum;
			sum = sum < 0 ? 0 : sum;
			output.at<uchar>(y, x) = saturate_cast<uchar>(sum);
		}
	}
	destinationImage = output.clone();
	return 1;
}

/*---------------------------------------------------------------------------*/
//Laplace

int EdgeDetector::detectByLaplace(const Mat& sourceImage, Mat& destinationImage)
{
	if (!sourceImage.data) return 0;

	Mat output = sourceImage.clone();
	int width = sourceImage.cols, height = sourceImage.rows;

	for (int y = 1; y < height - 1; y++)
	{
		for (int x = 1; x < width - 1; x++)
		{
			//normal
			/*int sum = -sourceImage.at<uchar>(y - 1, x)
				- sourceImage.at<uchar>(y + 1, x)
				- sourceImage.at<uchar>(y, x - 1)
				- sourceImage.at<uchar>(y, x + 1)
				+ 4 * sourceImage.at<uchar>(y, x);*/

				//include diagonals
			int sum =
				-sourceImage.at<uchar>(y - 1, x - 1)
				- sourceImage.at<uchar>(y - 1, x)
				- sourceImage.at<uchar>(y - 1, x + 1)

				- sourceImage.at<uchar>(y, x - 1)
				+ 8 * sourceImage.at<uchar>(y, x)
				- sourceImage.at<uchar>(y, x + 1)

				- sourceImage.at<uchar>(y + 1, x - 1)
				- sourceImage.at<uchar>(y + 1, x)
				- sourceImage.at<uchar>(y + 1, x + 1);

			sum = sum > 255 ? 255 : sum;
			sum = sum < 0 ? 0 : sum;

			output.at<uchar>(y, x) = saturate_cast<uchar>(sum);
		}
	}
	destinationImage = output.clone();
	return 1;
}

/*---------------------------------------------------------------------------*/
//Canny

int EdgeDetector::detectByCanny(const Mat& sourceImage, Mat& destinationImage, int low, int high)
{
	if (!sourceImage.data) return 0;

	float gx = 0, gy = 0, sum = 0;
	Mat output = sourceImage.clone();
	int width = sourceImage.cols, height = sourceImage.rows;

	cout << endl << width << endl;
	cout << endl << height << endl;

	//colR = column round; rowR = row round
	vector<int> colR;
	vector<vector<int>> rowR;

	//colC = column current; rowC = row current
	vector<double> colC;
	vector<vector<double>> rowC;

	for (int y = 1; y < height - 1; y++)
	{
		colR.clear();
		colC.clear();
		
		for (int x = 1; x < width - 1; x++)
		{
			//aplice Sobel kernel 
			gx = xGradient_Sobel(sourceImage, x, y);
			gy = yGradient_Sobel(sourceImage, x, y);

			// if (gx == 0) gx = 0.1;

			double edgeGradient = sqrt(gx * gx + gy * gy);

			double angle = (atan2(gy,gx) * 180 / 3.1415);
			colC.push_back(angle);

			// if(angle > 112.5 && angle <= 157.5)
			// cout << int(angle) << " ";

			if (angle <= 22.5 || angle >= 157.5) angle = 0;
			else if (angle > 22.5 && angle < 67.5) angle = 45;
			else if (angle >= 67.5 && angle <= 112.5) angle = 90;
			else if (angle > 112.5 && angle <= 157.5) angle = 135;

			// if(angle == 45)
			// cout << angle << " ";

			colR.push_back(angle);
		}
		rowC.push_back(colC);
		rowR.push_back(colR);
	}

	// col and row of non-maximum suppression
	vector<bool> colNMS;
	vector<vector<bool>> rowNMS;

	for (int y = 1; y < height - 3; y++)
	{
		for (int x = 1; x < width - 3; x++)
		{
			int value = rowC[y][x];

			if (rowR[y][x] == 45)
			{
				if (rowC[y][x] <= max(rowC[y - 1][x - 1], rowC[y + 1][x + 1])) value = 0;
			}
			else if (rowR[y][x] == 90)
			{
				if (rowC[y][x] <= max(rowC[y - 1][x], rowC[y + 1][x])) value = 0;
			}
			else if (rowR[y][x] == 135)
			{
				if (rowC[y][x] <= max(rowC[y - 1][x + 1], rowC[y + 1][x - 1])) value = 0;
			}
			else
			{
				if (rowC[y][x] <= max(rowC[y][x - 1], rowC[y][x + 1])) value = 0;
			}

			if (value > 100) value = 255;
			else value = 0;

			output.at<uchar>(y, x) = saturate_cast<uchar>(value);

		}
	}

	destinationImage = output.clone();
	return 1;
}
