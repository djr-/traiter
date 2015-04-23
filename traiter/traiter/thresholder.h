#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Traiter
{
	enum ThreshMethod;
}

namespace segment
{
	//////////////////////////////////////////////////////////////////////////////////
	// Thresholder
	//
	// Class used for thresolding an image.
	//////////////////////////////////////////////////////////////////////////////////
	class Thresholder final
	{
	public:
		static cv::Mat threshold(const cv::Mat& originalImage, const Traiter::ThreshMethod thresholdingMethod);
	private:
		static const int thresholdValue = 183;	//TODO_ROBUST: Allow user to alter this value.
		static const int thresholdType = cv::THRESH_BINARY;
		static const int maximumThresholdValue = 255;	//TODO_DESIGN: Perhaps this value should be set elsewhere and used here.
		static const int defaultBlockSize = 19;
	};
}