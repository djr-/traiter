#include "root_image_preprocessor.h"

using namespace cv;

//////////////////////////////////////////////////////////////////////////////////
// RootImagePreprocessor::PrepareForAnalysis()
//
// Prepares the image for analysis.
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::PrepareForAnalysis(Mat image)
{
	Mat processedImage = image.clone();

	//TODO: Add support for adaptive and double adaptive thresholding rather than always using global.
	threshold(processedImage, processedImage, thresholdValue, maximumThresholdValue, thresholdType);

	//TODO: Compute skeleton of the image.

	return processedImage;
}
