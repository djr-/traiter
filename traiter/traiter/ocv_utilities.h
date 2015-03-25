#pragma once

#include <opencv2/imgproc/imgproc.hpp>

//////////////////////////////////////////////////////////////////////////////////
// OcvUtilities
//
// Helper functions for common computations related to OpenCV.
//////////////////////////////////////////////////////////////////////////////////
class OcvUtilities final
{
public:
	static int getLargestContourIndex(const std::vector<std::vector<cv::Point>>& contours);
	static cv::RotatedRect computeBestFittingEllipse(const cv::Mat& image, const cv::Scalar maximumValue);
private:
	OcvUtilities();
};
