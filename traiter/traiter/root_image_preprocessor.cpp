#include "root_image_preprocessor.h"
#include "ocv_utilities.h"
#include "thresh_method.h"

using namespace cv;
using namespace Traiter;
using namespace Utility;

Mat RootImagePreprocessor::_removedContours;
vector<Point> RootImagePreprocessor::_rootContour;
Mat RootImagePreprocessor::_skeleton;

//////////////////////////////////////////////////////////////////////////////////
// prepareForAnalysis()
//
// Prepares the image for analysis.
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::prepareForAnalysis(Mat image)
{
	Mat processedImage = image.clone();

	ThreshMethod thresholdingMethod = ADAPTIVE_THRESH;	//TODO: This should be user specifiable.

	switch (thresholdingMethod)
	{
	case THRESH:
		threshold(processedImage, processedImage, thresholdValue, maximumThresholdValue, thresholdType);
		break;
	case ADAPTIVE_THRESH:
		adaptiveThreshold(processedImage, processedImage, maximumThresholdValue, ADAPTIVE_THRESH_MEAN_C, thresholdType, defaultBlockSize, 0);
		break;
	case DOUBLE_ADAPTIVE_THRESH:
		threshold(processedImage, processedImage, thresholdValue, maximumThresholdValue, thresholdType);	//TODO: Implement doubly adaptive thresholding.
		break;
	}

	keepOnlyLargestContour(processedImage);

	computeSkeleton(processedImage.clone());

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
// keepOnlyLargestContour()
//
// Remove all contours that are not the largest contour.
// Let's save a Mat of what was removed to removedContours to assist in debugging
//
// TODO_ROBUST: We may want to only remove the contours outside of the bounding rect.
//		 So only cleanup items inside boundingRect(contours[largestContourIndex])
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::keepOnlyLargestContour(Mat image)
{
	_removedContours = image.clone();

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

	_rootContour = contours[largestContourIndex];	// Cache the contour for later analysis

	_removedContours = _removedContours - image;

	return image;
}

//////////////////////////////////////////////////////////////////////////////////
// computeSkeleton()
//
// Compute the morphological skeleton of the image. This algorithm is based on
// the skeletonization method described in Digital Image Processing by
// Gonzalez and Woods 3rd Edition (page 651-654).
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::computeSkeleton(Mat image)
{
	_skeleton = Mat(image.size(), CV_8UC1, Scalar(0));
	Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
	Mat erodedImage;
	Mat openedImage;
	
	do
	{
		erode(image, erodedImage, element);
		dilate(erodedImage, openedImage, element);
		subtract(image, openedImage, openedImage);	// Note: At this point the variable "openedImage" is not exactly the opened image, but we can use it as a temporary store.
		bitwise_or(_skeleton, openedImage, _skeleton);
		erodedImage.copyTo(image);
		
	} while (!countNonZero(openedImage) == 0);

	return _skeleton;
}

//////////////////////////////////////////////////////////////////////////////////
// getRemovedContours()
//
// Returns an image showing which contours were removed from the specified image.
// Note that this does not return a cloned version of removedContours!
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::getRemovedContours()
{
	return _removedContours;
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

//////////////////////////////////////////////////////////////////////////////////
// getSkeleton()
//
// Returns an image representing the skeleton of the image.
//////////////////////////////////////////////////////////////////////////////////
Mat RootImagePreprocessor::getSkeleton()
{
	return _skeleton;
}
