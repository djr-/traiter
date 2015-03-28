#include "root_system.h"
#include "root_image_preprocessor.h"
#include "ocv_utilities.h"
#include <opencv2/opencv.hpp>

using namespace cv;

//////////////////////////////////////////////////////////////////////////////////
// RootSystem::RootSystem()
//
// Constructor to specify the image to compute the root system from.
//////////////////////////////////////////////////////////////////////////////////
RootSystem::RootSystem(Mat image)
{
	_image = RootImagePreprocessor::prepareForAnalysis(image);
	_contour = RootImagePreprocessor::getRootContour();

	//TODO: Calling getRootContour() is ugly as it depends on prepareForAnalysis to be called first. We may want to precompute the contours,
	//      and pull some of the functionality from RootImagePreprocessor into OcvUtilities. For now, just leave it as is in order to finish first pass of trait computation.
}

//////////////////////////////////////////////////////////////////////////////////
// RootSystem::getImage()
//
// Returns the image that we are operating on.
//////////////////////////////////////////////////////////////////////////////////
cv::Mat RootSystem::getImage()
{
	return _image.clone();
}

//////////////////////////////////////////////////////////////////////////////////
// bushiness()
//
// Units: n / n
//
// The ratio of the maximum to the median number of roots.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::bushiness()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// convexArea()
//
// Units: cm^2
//
// The area of the convex hull that encompasses the root.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::convexArea()
{
	vector<Point> hull;
	convexHull(_contour, hull);

	//TODO: Make a debugging flag to write out an image if the flag is turned on?
	//drawContours(_image, vector<vector<Point>>(1, hull), 0, Scalar(255));	// drawContours expects a vector of vectors, so we need to construct the expected type from our largest hull contour.

	return contourArea(hull);
}

//////////////////////////////////////////////////////////////////////////////////
// networkDepth()
//
// Units: cm
//
// The number of pixels in the vertical direction from the upper-most network
// pixel to the lower-most network pixel.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::networkDepth()
{
	return 0;

	//TODO: Find the lowest y-value in the contour minus the highest y-value in the contour.
}

//////////////////////////////////////////////////////////////////////////////////
// networkLengthDistribution()
//
// Units: n / n
//
// The fraction of network pixels found in the lower 2/3 of the network. The
// lower 2/3 of the network is defined based on the network depth.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::networkLengthDistribution()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// majorAxis()
//
// Units: cm
//
// The length of the major axis of the best fitting ellipse to the network.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::majorAxis()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// networkWidth()
//
// Units: cm
//
// The number of pixels in the horizontal direction from the left-most network
// pixel to the right-most network pixel. Only pixels lying in the same row are
// considered.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::networkWidth()
{
	return 0;

	//TODO: Find the highest x-value minus the lowest x-value.
}

//////////////////////////////////////////////////////////////////////////////////
// maximumNumberOfRoots()
//
// Units: n
//
// After sorting the number of roots crossing a horizontal line from smallest to
// largest, the maximum number is considered to be the 84th-percentile value
// (one standard deviation).
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::maximumNumberOfRoots()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// averageRootWidth()
//
// Units: cm
//
// The mean value of the root width estimation computed for all pixels of the
// medial axis of the entire root system. This trait corresponds to diameter
// of a root.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::averageRootWidth()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// medianNumberOfRoots()
//
// Units: n
//
// The result of a vertical line sweep in which the number of roots that crossed
// a horizontal line was estimated, and then the median of all values for the
// extent of the network was calculated.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::medianNumberOfRoots()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// minorAxis()
//
// Units: cm
//
// The length of the minor axis of the best fitting ellipse to the network.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::minorAxis()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// networkArea()
//
// Units: cm^2
//
// The number of network pixels in the image.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::networkArea()
{
	double networkArea = 0;

	MatIterator_<uchar> it, end;
	for (it = _image.begin<uchar>(), end = _image.end<uchar>(); it != end; ++it)
	{
		if (*it == RootImagePreprocessor::getMaximumThresholdValue())
		{
			networkArea++;
		}
	}

	//TODO: Consider using countNonZero(_image) here, although may not be useful for images that have different threshold settings.

	return networkArea;	//TODO: Is there a way we can use the contour rather than the entire image here?
}

//////////////////////////////////////////////////////////////////////////////////
// perimeter()
//
// Units: cm
//
// The total number of network pixels connected to a background pixel (using a
// 8-nearest neighbor neighborhood).
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::perimeter()
{
	double perimeter = 0;

	for (int row = 0; row < _image.size().height; ++row)
	{
		for (int col = 0; col < _image.size().width; ++col)
		{
			if (OcvUtilities::isPointWhite(_image, Point(col, row)))
			{
				vector<Point> neighbors = OcvUtilities::getNeighboringPixels(_image, Point(col, row));

				bool allWhiteNeighbors = true;
				
				for (vector<Point>::iterator it = neighbors.begin(); it != neighbors.end(); ++it)
				{
					if (!OcvUtilities::isPointWhite(_image, *it))
					{
						allWhiteNeighbors = false;
					}
				}

				if (allWhiteNeighbors)
					perimeter++;
			}
		}
	}

	return perimeter;
}

//////////////////////////////////////////////////////////////////////////////////
// aspectRatio()
//
// Units: cm/cm
//
// The ratio of the minor to the major axis of best fitting ellipse.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::aspectRatio()
{
	RotatedRect bestFittingEllipse = fitEllipse(_contour);

	//TODO: Make a debugging flag to write out an image if the flag is turned on?
	//ellipse(_image, bestFittingEllipse, Scalar(255));

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// networkSolidity()
//
// Units: cm^2/cm^2
//
// The total network area divided by the network convex area.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::networkSolidity()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// specificRootLength()
//
// Units: cm/cm^3
//
// Total network length divided by network volume. Volume is estimated as the sum
// of cross-sectional areas for all pixels of the medial axis of the root system.
// The total root length is the number of pixels in the medial axis of the root
// system.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::specificRootLength()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// networkSurfaceArea()
//
// Units: cm^2
//
// The sum of the local surface area at each pixel of th network skeleton, as
// approximated by a tubular shape whose radius is estimated from the image.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::networkSurfaceArea()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// networkLength()
//
// Units: cm
//
// The total number of pixels in the network skeleton.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::networkLength()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// networkVolume()
//
// Units: cm^3
//
// The sum of the local volume at each pixel of the network skeleton, as
// approximated by a tubular shape whose radius is estimated from the image.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::networkVolume()
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
// networkWidthToDepthRatio()
//
// Units: cm/cm
//
// The value of network width divided by the value of network depth.
//////////////////////////////////////////////////////////////////////////////////
double RootSystem::networkWidthToDepthRatio()
{
	return 0;
}
