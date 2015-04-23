#include "root_image_preprocessor.h"
#include "ocv_utilities.h"
#include "thresh_method.h"

using namespace cv;
using namespace Traiter;
using namespace Utility;

vector<Point> RootImagePreprocessor::_rootContour;

//////////////////////////////////////////////////////////////////////////////////
// prepareForAnalysis()
//
// Prepares the image for analysis.
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::prepareForAnalysis(Mat image)
{
	Mat processedImage = image.clone();

	keepOnlyLargestContour(processedImage);

	return processedImage;
}

//////////////////////////////////////////////////////////////////////////////////
// keepOnlyLargestContour()
//
// Remove all contours that are not the largest contour.
//
// TODO_ROBUST: We may want to only remove the contours outside of the bounding rect.
//		 So only cleanup items inside boundingRect(contours[largestContourIndex])
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::keepOnlyLargestContour(Mat image)
{
	Mat largestContour;
	OcvUtilities::padImage(image, largestContour);	// If we don't pad, then findContours will not mark the edge as part of the contour. We will fix then when we draw the contours on the original image.

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(largestContour, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

	int largestContourIndex = OcvUtilities::getLargestContourIndex(contours);

	image = image.zeros(image.size(), CV_8UC1);	// Clear the existing image before drawing the largest contour back onto it.
	largestContour = largestContour.zeros(largestContour.size(), CV_8UC1);

	drawContours(largestContour, contours, largestContourIndex, Scalar(255), CV_FILLED, 8, hierarchy);	//TODO: Instead of 255, use maximumThresholdValue

	OcvUtilities::removePadding(largestContour, image);

	_rootContour = contours[largestContourIndex];	// Cache the contour for later analysis

	return image;
}

//////////////////////////////////////////////////////////////////////////////////
// getRootContour()
//
// Returns the contour representing the root image that was produced.
//////////////////////////////////////////////////////////////////////////////////
vector<Point> RootImagePreprocessor::getRootContour()
{
	return _rootContour;
}
