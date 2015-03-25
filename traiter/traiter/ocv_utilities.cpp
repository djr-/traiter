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
// computeBestFittingEllipse()
//
// Computes the best fitting ellipse for the specified image, and returns the
// result as a RotatedRect.
//
// Does not alter the passed-in image.
//////////////////////////////////////////////////////////////////////////////////
RotatedRect OcvUtilities::computeBestFittingEllipse(const Mat& image, const Scalar maximumValue = Scalar(255, 255, 255))
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(image.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	int largestContourIndex = OcvUtilities::getLargestContourIndex(contours);

	RotatedRect bestFittingEllipse = fitEllipse(Mat(contours[largestContourIndex]));
	
	//TODO: Temporary notes for debugging. We need a way to compute major/minor axis from this information.
	//Mat drawing = Mat::zeros(image.size(), CV_8UC1);
	//ellipse(drawing, bestFittingEllipse, maximumValue);
	//RotatedRect minRect = minAreaRect(Mat(contours[largestContourIndex]));
	//rectangle(drawing, bestFittingEllipse.boundingRect(), maximumValue);

	return bestFittingEllipse;
}
