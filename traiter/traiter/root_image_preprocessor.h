#pragma once

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
	static cv::Mat prepareForAnalysis(cv::Mat image);
	static int getMaximumThresholdValue();
	static std::vector<cv::Point> getRootContour();

	// Debugging/analysis functions.
	static cv::Mat getRemovedContours();
private:
	RootImagePreprocessor();

	// Helper functions to prepare image for analysis
	static cv::Mat thresholdImage(cv::Mat image);
	static cv::Mat keepOnlyLargestContour(cv::Mat image);

	// Helpful data to cache for analysis
	static std::vector<cv::Point> rootContour;

	// Data useful for debugging/analysis
	static cv::Mat removedContours;

	// Default values for thresholding.
	static const int thresholdValue = 176;
	static const int thresholdType = cv::THRESH_BINARY;
	static const int maximumThresholdValue = 255;
};
