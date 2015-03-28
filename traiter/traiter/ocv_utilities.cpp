#include "ocv_utilities.h"

using namespace std;
using namespace cv;

//////////////////////////////////////////////////////////////////////////////////
// getLargestContourIndex()
//
// Find the largest contour in the list of contours and return its index.
//////////////////////////////////////////////////////////////////////////////////
int OcvUtilities::getLargestContourIndex(const vector<vector<Point>>& contours)
{
	int largestContourIndex = 0;
	double largestContourArea = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		double currentContourArea = contourArea(contours[i]);
		if (currentContourArea > largestContourArea)
		{
			largestContourArea = currentContourArea;
			largestContourIndex = i;
		}
	}

	return largestContourIndex;
}

//////////////////////////////////////////////////////////////////////////////////
// isPointInImage()
//
// Returns true if the specified point is inside the image, false otherwise.
//////////////////////////////////////////////////////////////////////////////////
bool OcvUtilities::isPointInImage(const Mat& image, const Point& point)
{
	return point.inside(Rect(0, 0, image.size().width, image.size().height));
}

//////////////////////////////////////////////////////////////////////////////////
// isPointExpectedColor()
//
// Returns true if the specified point is both in the image and is the specified
// color.
//////////////////////////////////////////////////////////////////////////////////
template<typename T>
bool OcvUtilities::isPointExpectedColor(const Mat& image, const Point& point, const T color)
{
	if (!isPointInImage(image, point))
	{
		assert(false);
		return false;
	}

	return image.at<T>(point) == color;
}

//////////////////////////////////////////////////////////////////////////////////
// isPointWhite()
//
// Returns true if the specified point is both in the image and is white.
// Note that this function assumes we are representing our pixels as a uchar.
//////////////////////////////////////////////////////////////////////////////////
bool OcvUtilities::isPointWhite(const Mat& image, const Point& point)
{
	return isPointExpectedColor<uchar>(image, point, 255);
}

//////////////////////////////////////////////////////////////////////////////////
// getNeighboringPixels()
//
// Returns a list of the 8 neighboring pixels that are in the image.
//////////////////////////////////////////////////////////////////////////////////
vector<Point> OcvUtilities::getNeighboringPixels(const Mat& image, const Point& point)
{
	vector<Point> neighboringPixels;

	for (int x = point.x - 1; x <= point.x + 1; ++x)
	{
		for (int y = point.y - 1; y <= point.y + 1; ++y)
		{
			Point neighbor(x, y);
			
			if (neighbor == point)
				continue;	// Don't add yourself as a neighbor.

			if (OcvUtilities::isPointInImage(image, neighbor))
				neighboringPixels.push_back(neighbor);
		}
	}

	return neighboringPixels;
}

//////////////////////////////////////////////////////////////////////////////////
// padImage()
//
// Pads the image by the specified padding amount with the default value (0).
//////////////////////////////////////////////////////////////////////////////////
void OcvUtilities::padImage(const Mat& sourceImage, Mat& destinationImage, const int padAmount)
{
	copyMakeBorder(sourceImage, destinationImage, 1, 1, 1, 1, BORDER_CONSTANT);
}

//////////////////////////////////////////////////////////////////////////////////
// removePadding()
//
// Removes borders from the specified image by the specified padding amount.
//////////////////////////////////////////////////////////////////////////////////
void OcvUtilities::removePadding(const cv::Mat& sourceImage, cv::Mat& destinationImage, const int padAmount)
{
	sourceImage(Rect(padAmount, padAmount, sourceImage.size().width - 1 - padAmount, sourceImage.size().height - 1 - padAmount)).copyTo(destinationImage);
}
