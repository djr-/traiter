#include "root_image_preprocessor.h"

using namespace cv;

//////////////////////////////////////////////////////////////////////////////////
// RootImagePreprocessor::prepareForAnalysis()
//
// Prepares the image for analysis.
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::prepareForAnalysis(Mat image)
{
	Mat processedImage = image.clone();

	//TODO: Add support for adaptive and double adaptive thresholding rather than always using global.
	threshold(processedImage, processedImage, thresholdValue, maximumThresholdValue, thresholdType);

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