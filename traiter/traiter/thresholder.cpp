#include "thresholder.h"
#include "thresh_method.h"
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace segment;
using namespace Traiter;

//////////////////////////////////////////////////////////////////////////////////
// threshold()
//
// Thresholds the image according to the specified method, and returns the
// thresholded image.
//////////////////////////////////////////////////////////////////////////////////
Mat Thresholder::threshold(const Mat& originalImage, const Traiter::ThreshMethod thresholdingMethod)
{
	Mat image = originalImage.clone();

	Mat thresholdImage;

	switch (thresholdingMethod)
	{
	case THRESH:
		cv::threshold(image, thresholdImage, thresholdValue, maximumThresholdValue, thresholdType);
		break;
	case ADAPTIVE_THRESH:
		cv::adaptiveThreshold(image, thresholdImage, maximumThresholdValue, ADAPTIVE_THRESH_MEAN_C, thresholdType, defaultBlockSize, 0);
		break;
	case DOUBLE_ADAPTIVE_THRESH:
		cv::threshold(image, thresholdImage, thresholdValue, maximumThresholdValue, thresholdType);	//TODO: Implement doubly adaptive thresholding.
		break;
	}

	return thresholdImage;
}
