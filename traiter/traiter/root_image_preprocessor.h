#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Traiter
{
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
		static cv::Mat getSkeleton();

		// Debugging/analysis functions.
		static cv::Mat getRemovedContours();
	private:
		RootImagePreprocessor();

		// Helper functions to prepare image for analysis
		static cv::Mat keepOnlyLargestContour(cv::Mat image);
		static cv::Mat computeSkeleton(cv::Mat image);

		// Helpful data to cache for analysis
		static std::vector<cv::Point> _rootContour;
		static cv::Mat _skeleton;

		// Data useful for debugging/analysis
		static cv::Mat _removedContours;

		// Default values for thresholding.
		static const int thresholdValue = 183;	//TODO: 180 seems to match. 176 works better...
		static const int thresholdType = cv::THRESH_BINARY;
		static const int maximumThresholdValue = 255;
		static const int defaultBlockSize = 19;
	};

}
