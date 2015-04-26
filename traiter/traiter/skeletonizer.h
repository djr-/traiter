#pragma once

#include <opencv2/core/core.hpp>

namespace morph
{
	//////////////////////////////////////////////////////////////////////////////////
	// Skeletonizer
	//
	// Class used for computing the skeleton of a specified image.
	//////////////////////////////////////////////////////////////////////////////////
	class Skeletonizer final
	{
	public:
		static cv::Mat computeMorphologicalSkeleton(const cv::Mat& originalImage);
		static cv::Mat computeMedialAxisTransform(const cv::Mat& originalImage);
	};
}
