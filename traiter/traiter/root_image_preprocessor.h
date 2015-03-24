#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//////////////////////////////////////////////////////////////////////////////////
// RootImagePreprocessor
//
// This class ensures that before any traits have been estimated, the image has
// been processed sufficiently.
//////////////////////////////////////////////////////////////////////////////////
class RootImagePreprocessor final
{
public:
	static cv::Mat PrepareForAnalysis(cv::Mat image);
private:
	RootImagePreprocessor();

	static const int thresholdValue = 180;

	static const int thresholdType = cv::THRESH_BINARY_INV;
	static const int maximumThresholdValue = 255;
};
