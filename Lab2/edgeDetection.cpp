#include "edgeDetection.h"

EdgeDetector::EdgeDetector() {}
EdgeDetector::~EdgeDetector() {}

/*---------------------------------------------------------------------------*/

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

int EdgeDetector::detectByCany(const Mat& sourceImage, Mat& destinationImage)
{
	if (!sourceImage.data) return 0;

	int gx = 0, gy = 0, sum = 0;
	Mat output = sourceImage.clone();
	int width = sourceImage.cols, height = sourceImage.rows;

	cout << endl << width << endl;
	cout << endl << height << endl;

	vector<int> col;
	vector<vector<int>> row;

	for (int y = 1; y < height - 1; y++)
	{
		col.clear();
		// row.resize(0);
		for (int x = 1; x < width - 1; x++)
		{
			gx = xGradient_Sobel(sourceImage, x, y);
			gy = yGradient_Sobel(sourceImage, x, y);

			double edgeGradient = sqrt(gx * gx + gy * gy);

			double angle = abs(atan(edgeGradient) * 180 / 3.1415);

			if (angle <= 22.5 || angle >= 157.5) angle = 0;
			else if (angle > 22.5 || angle < 67.5) angle = 45;
			else if (angle >= 67.5 || angle <= 112.5) angle = 90;
			else if (angle > 112.5 || angle <= 157.5) angle = 135;
			else angle = 0;

			col.push_back(angle);
		}
		row.push_back(col);
	}

	cout << endl << col.size() << endl;

	cout << endl << row.size() << endl;

	int value = 0;

	for(int y=1;y<height-3;y++)
	{
		for(int x=1;x<width-3;x++)
		{
			if (row[y][x]==45)
			{
				if (row[y][x] >= max(row[y-1][x-1], row[y+1][x+1])) value = 255;
				else value = 0;
			}
			else if (row[y][x] == 90)
			{
				if (row[y][x] >= max(row[y - 1][x], row[y + 1][x])) value = 255;
				else value = 0;
			}
			else if (row[y][x] == 135)
			{
				if (row[y][x] >= max(row[y - 1][x + 1], row[y + 1][x - 1])) value = 255;
				else value = 0;
			}
			else
			{
				if (row[y][x] >= max(row[y][x - 1], row[y][x + 1])) value = 255;
				else value = 0;
			}

			output.at<uchar>(y, x) = saturate_cast<uchar>(value);

		}
	}

	destinationImage = output.clone();
	return 1;
}
