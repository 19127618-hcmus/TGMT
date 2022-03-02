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

			//Sobel in both the x and y directions.
			sum = abs(gx) + abs(gy);

			//sum must be in range [0, 255]
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
//same as Sobel. Different from Sobel in the kernel.
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
			//normal kernel
			/*int sum = -sourceImage.at<uchar>(y - 1, x)
				- sourceImage.at<uchar>(y + 1, x)
				- sourceImage.at<uchar>(y, x - 1)
				- sourceImage.at<uchar>(y, x + 1)
				+ 4 * sourceImage.at<uchar>(y, x);*/

			//another type of kernel: direct multiplication.
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

	float gx = 0, gy = 0;
	Mat output = sourceImage.clone();
	int width = sourceImage.cols, height = sourceImage.rows;

	//col and row current
	vector<double> colC;
	vector<vector<double>> rowC;

	//col and row round
	vector<int> colR;
	vector<vector<int>> rowR;

	int Gmax = 0; // gradient max

	//calculate intensity gradient
	for (int y = 1; y < height - 1; y++)
	{
		colR.clear();
		colC.clear();

		for (int x = 1; x < width - 1; x++)
		{
			//aplice Sobel kernel 
			gx = xGradient_Sobel(sourceImage, x, y);
			gy = yGradient_Sobel(sourceImage, x, y);

			double edgeGradient = sqrt(gx * gx + gy * gy);

			//find gradient max
			if (edgeGradient > Gmax)
				Gmax = edgeGradient;

			colC.push_back(edgeGradient);

			//find angle as degrees format
			double angle = (atan2(gy, gx) * 180 / 3.1415);

			//round angle become 0, 45, 90, 135
			if ((angle >= 0 && angle < 22.5)|| (angle > 157.5 && angle < 180)|| (angle > -22.5 && angle < 0)|| (angle > -180 && angle < -157.5)) angle = 0;
			else if ((angle > 22.5 && angle < 67.5)|| (angle > -157.5 && angle < -112.5)) angle = 45;
			else if ((angle >= 67.5 && angle <= 112.5) || (angle > -112.5 && angle < -67.5)) angle = 90;
			else if ((angle > 112.5 && angle <= 157.5) || (angle > -67.5 && angle < -22.5)) angle = 135;

			colR.push_back(angle);
		}
		rowC.push_back(colC);
		rowR.push_back(colR);
	}

	// low and high condition
	if (height > Gmax) height = Gmax;
	if (high < 0) high = 0;
	if (low < 0) low = 0;
	if (low > high) low = high;

	//Suppression of false edges //non-maximum suppression
	for (int y = 1; y < height - 3; y++)
	{
		for (int x = 1; x < width - 3; x++)
		{
			int value = rowC[y][x];

			//If neighboring pixels in the specified direction are larger than the current pixel, that pixel is not an edge -> set it into 0.
			if (rowR[y][x] == 45)
			{
				if (rowC[y][x] < max(rowC[y - 1][x - 1], rowC[y + 1][x + 1])) value = 0;
			}
			else if (rowR[y][x] == 90)
			{
				if (rowC[y][x] < max(rowC[y - 1][x], rowC[y + 1][x])) value = 0;
			}
			else if (rowR[y][x] == 135)
			{
				if (rowC[y][x] < max(rowC[y - 1][x + 1], rowC[y + 1][x - 1])) value = 0;
			}
			else
			{
				if (rowC[y][x] < max(rowC[y][x - 1], rowC[y][x + 1])) value = 0;
			}

			//classify: strong edge, not edge and weak edge
			if (value >= high) value = 255;
			else if (value < low) value = 0;
			else value = 75;
			output.at<uchar>(y, x) = saturate_cast<uchar>(value);

		}
	}
	int non_max_height = output.rows;
	int non_max_width = output.cols;

	//hysteresis thresholding
	//is weak edge a strong edge?
	for (int y = 1; y < non_max_height - 2; y++)
	{
		for (int x = 1; x < non_max_width - 2; x++)
		{
			if (output.at<uchar>(y, x) == 75)
			{
				//if this pixel has a neighbor is strong edge -> it is strong edge
				//else it is not edge
				if ((output.at<uchar>(y - 1, x - 1) == 255) || (output.at<uchar>(y - 1, x) == 255) || (output.at<uchar>(y - 1, x + 1) == 255) ||
					(output.at<uchar>(y, x - 1) == 255) || (output.at<uchar>(y, x + 1) == 255) ||
					(output.at<uchar>(y + 1, x - 1) == 255) || (output.at<uchar>(y + 1, x) == 255) || (output.at<uchar>(y + 1, x + 1) == 255))
				{
					output.at<uchar>(y, x) = saturate_cast < uchar>(255);
				}
				else output.at<uchar>(y, x) = saturate_cast < uchar>(0);
			}
		}
	}

	destinationImage = output.clone();
	return 1;
}
