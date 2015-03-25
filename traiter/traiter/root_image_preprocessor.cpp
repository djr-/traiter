#include "root_image_preprocessor.h"

using namespace cv;

Mat RootImagePreprocessor::removedContours;

//////////////////////////////////////////////////////////////////////////////////
// RootImagePreprocessor::prepareForAnalysis()
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
// RootImagePreprocessor::getMaximumThresholdValue()
//
// Returns the maximum threshold value that was used to threshold the image.
//////////////////////////////////////////////////////////////////////////////////
int RootImagePreprocessor::getMaximumThresholdValue()
{
	return maximumThresholdValue;
}

//////////////////////////////////////////////////////////////////////////////////
// RootImagePreprocessor::thresholdImage()
//
// Thresholds the image according to the configurations threshold parameters.
// TODO: Add support for adaptive and double adaptive thresholding rather than always using global.
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::thresholdImage(Mat image)
{
	threshold(image, image, thresholdValue, maximumThresholdValue, thresholdType);

	return image;
}

//////////////////////////////////////////////////////////////////////////////////
// RootImagePreprocessor::keepOnlyLargestContour()
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
	copyMakeBorder(image, largestContour, 1, 1, 1, 1, BORDER_CONSTANT);	// If we don't pad, then findContours will not mark the edge as part of the contour. We will fix then when we draw the contours on the original image.

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(largestContour, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

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

	image = image.zeros(image.size(), CV_8UC1);	// Clear the existing image before drawing the largest contour back onto it.

	drawContours(image, contours, largestContourIndex, Scalar(maximumThresholdValue), CV_FILLED, 8, hierarchy);

	removedContours = removedContours - image;

	return image;
}

//////////////////////////////////////////////////////////////////////////////////
// RootImagePreprocessor::getRemovedContours()
//
// Returns an image showing which contours were removed from the specified image.
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::getRemovedContours()
{
	return removedContours;
}
