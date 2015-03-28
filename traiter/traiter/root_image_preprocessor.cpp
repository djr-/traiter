#include "root_image_preprocessor.h"
#include "ocv_utilities.h"

using namespace cv;

Mat RootImagePreprocessor::removedContours;
vector<Point> RootImagePreprocessor::rootContour;

//////////////////////////////////////////////////////////////////////////////////
// prepareForAnalysis()
//
// Prepares the image for analysis.
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::prepareForAnalysis(Mat image)
{
	Mat processedImage = image.clone();

	thresholdImage(processedImage);

	keepOnlyLargestContour(processedImage);

	//TODO: Compute skeleton of the image.

	return processedImage;
}

//////////////////////////////////////////////////////////////////////////////////
// getMaximumThresholdValue()
//
// Returns the maximum threshold value that was used to threshold the image.
//////////////////////////////////////////////////////////////////////////////////
int RootImagePreprocessor::getMaximumThresholdValue()
{
	return maximumThresholdValue;
}

//////////////////////////////////////////////////////////////////////////////////
// thresholdImage()
//
// Thresholds the image according to the configurations threshold parameters.
//
// TODO: Add support for adaptive and double adaptive thresholding rather than always using global.
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::thresholdImage(Mat image)
{
	threshold(image, image, thresholdValue, maximumThresholdValue, thresholdType);

	return image;
}

//////////////////////////////////////////////////////////////////////////////////
// keepOnlyLargestContour()
//
// Remove all contours that are not the largest contour.
// Let's save a Mat of what was removed to removedContours to assist in debugging
//
// TODO: We may want to only remove the contours outside of the bounding rect.
//		 So only cleanup items inside boundingRect(contours[largestContourIndex])
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::keepOnlyLargestContour(Mat image)
{
	removedContours = image.clone();

	Mat largestContour;
	OcvUtilities::padImage(image, largestContour);	// If we don't pad, then findContours will not mark the edge as part of the contour. We will fix then when we draw the contours on the original image.

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(largestContour, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

	int largestContourIndex = OcvUtilities::getLargestContourIndex(contours);

	image = image.zeros(image.size(), CV_8UC1);	// Clear the existing image before drawing the largest contour back onto it.
	largestContour = largestContour.zeros(largestContour.size(), CV_8UC1);

	drawContours(largestContour, contours, largestContourIndex, Scalar(maximumThresholdValue), CV_FILLED, 8, hierarchy);

	OcvUtilities::removePadding(largestContour, image);

	rootContour = contours[largestContourIndex];	// Cache the contour for later analysis

	removedContours = removedContours - image;

	return image;
}

//////////////////////////////////////////////////////////////////////////////////
// getRemovedContours()
//
// Returns an image showing which contours were removed from the specified image.
// Note that this does not return a cloned version of removedContours!
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::getRemovedContours()
{
	return removedContours;
}

//////////////////////////////////////////////////////////////////////////////////
// getRootContour()
//
// Returns the contour representing the root image that was produced.
//////////////////////////////////////////////////////////////////////////////////
vector<Point> RootImagePreprocessor::getRootContour()
{
	return rootContour;	//TODO: Confirm that we are not off-by-one pixel in the contour as opposed to the image (from padding/removal)
}