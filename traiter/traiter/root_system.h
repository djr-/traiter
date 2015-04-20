#pragma once

#include <opencv2/core/core.hpp>

//////////////////////////////////////////////////////////////////////////////////
// RootSystem
//
// Computes traits for the specified root system image.
//////////////////////////////////////////////////////////////////////////////////
class RootSystem
{
public:
	RootSystem(cv::Mat image);

	cv::Mat getImage();

	// Traits
	double bushiness();
	double convexArea();
	double networkDepth();
	double networkLengthDistribution();
	double majorAxis();
	double networkWidth();
	double maximumNumberOfRoots();
	double averageRootWidth();
	double medianNumberOfRoots();
	double minorAxis();
	double networkArea();
	double perimeter();
	double aspectRatio();
	double networkSolidity();
	double specificRootLength();
	double networkSurfaceArea();
	double networkLength();
	double networkVolume();
	double networkWidthToDepthRatio();
private:
	RootSystem();

	std::vector<int> computeNumberOfRootsInRows(bool includeZeroes = false);

	cv::Mat _image;
	std::vector<cv::Point> _contour;
	cv::Mat _skeleton;
};
