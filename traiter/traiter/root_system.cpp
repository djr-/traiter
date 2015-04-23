#include "root_system.h"
#include "general_utilities.h"
#include "ocv_utilities.h"
#include "root_image_preprocessor.h"
#include "skeletonizer.h"
#include "thresh_method.h"
#include "thresholder.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
using namespace Traiter;
using namespace Utility;

//////////////////////////////////////////////////////////////////////////////////
// RootSystem::RootSystem()
//
// Constructor to specify the image to compute the root system from.
//////////////////////////////////////////////////////////////////////////////////
RootSystem::RootSystem(Mat image)
{
	_image = segment::Thresholder::threshold(image, ADAPTIVE_THRESH);
	_image = RootImagePreprocessor::prepareForAnalysis(image);
	_contour = RootImagePreprocessor::getRootContour();
	_skeleton = morph::Skeletonizer::computeMorphologicalSkeleton(_image);

	//TODO_DESIGN: Calling getRootContour() is ugly as it depends on prepareForAnalysis to be called first. We may want to precompute the contours,
	//      and pull some of the functionality from RootImagePreprocessor into OcvUtilities. For now, just leave it as is in order to finish first pass of trait computation.

	//TODO_PERF: We may want to consider only iterating through the image once, and computing all of these in one pass. This would speed things up significantly

	//TODO_ROBUST: Consider a set of defensive checks here to verify _image and _contour were constructed properly.
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
	return maximumNumberOfRoots() / medianNumberOfRoots();	//TODO_ROBUST: Check for division by zero here and below.
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

	//TODO_ROBUST: Make a debugging flag to write out an image if the flag is turned on?
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
	if (_contour.size() < 1)
		return -1;

	int minY = _contour[0].y;
	int maxY = _contour[0].y;

	for each (auto point in _contour)
	{
		if (point.y > maxY)
			maxY = point.y;
		if (point.y < minY)
			minY = point.y;
	}

	return maxY - minY;
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
	int lowerTwoThirdsStart = static_cast<int>(round(0.33 * networkDepth()));	//TODO: Network depth computation makes sense here only if the top of the image is where the network starts (true in current sample image case)

	double lowerTwoThirdsArea = 0;

	for (int row = lowerTwoThirdsStart; row <= networkDepth(); ++row)
	{
		for (int col = 0; col < _image.size().width; ++col)
		{
			if (OcvUtilities::isPointWhite(_image, Point(col, row)))
			{
				lowerTwoThirdsArea++;
			}
		}
	}

	//TODO_ROBUST: Allow debugging image to be output.
	//for (int col = 0; col < _image.size().width; ++col)
	//{
	//	_image.at<uchar>(Point(col, lowerTwoThirdsStart)) = 255;
	//	imshow("Lower Two-Thirds Line", _image);
	//}

	return lowerTwoThirdsArea / networkArea();	//TODO_PERF: Don't iterate through 2/3 of the image twice to compute this.
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
	RotatedRect bestFittingEllipse = fitEllipse(_contour);

	return round(max(bestFittingEllipse.size.width, bestFittingEllipse.size.height));
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
	if (_contour.size() < 1)
		return -1;

	int minX = _contour[0].x;
	int maxX = _contour[0].x;

	for each (auto point in _contour)
	{
		if (point.x > maxX)
			maxX = point.x;
		if (point.x < minX)
			minX = point.x;
	}

	return maxX - minX;	//TODO: Note that this does not assume that pixels are in the same row as specified in the comment above. We should confirm that this is the desired behavior.
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
	const double PERCENTILE = 0.84;

	vector<int> numberOfRootsInRow = computeNumberOfRootsInRows();

	sort(numberOfRootsInRow.begin(), numberOfRootsInRow.end());

	int maximumIndex = static_cast<int>(round(PERCENTILE * numberOfRootsInRow.size()));

	return numberOfRootsInRow[maximumIndex];
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
	return GeneralUtilities::computeMedian(computeNumberOfRootsInRows());
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
	RotatedRect bestFittingEllipse = fitEllipse(_contour);	//TODO_PERF: Compute the best fitting ellipse only once.

	return round(min(bestFittingEllipse.size.width, bestFittingEllipse.size.height));
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
	return countNonZero(_image);	//TODO_DESIGN: Is there a way we can use the contour rather than the entire image here?
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
	return minorAxis() / majorAxis();
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
	return networkArea() / convexArea();	//TODO: networkArea is computed based on pixels, convexArea is computed based on the contour. This ratio might not be apples to apples...
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
// The sum of the local surface area at each pixel of the network skeleton, as
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
	return countNonZero(_skeleton);
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
	return networkWidth() / networkDepth();
}

//////////////////////////////////////////////////////////////////////////////////
// computeNumberOfRootsInRow()
//
// Returns a list of integers corresponding to the number of roots that were found
// in each row of the image. A root can be considered to be "found" when we find
// a white pixel when the previous pixel was black.
//////////////////////////////////////////////////////////////////////////////////
vector<int> RootSystem::computeNumberOfRootsInRows(bool includeZeroes)	//TODO_PERF: Compute this only once, rather than once for both maxmimum and minimum.
{
	vector<int> numberOfRootsInRows(_image.size().height);

	for (int row = 0; row < _image.size().height; ++row)
	{
		for (int col = 0; col < _image.size().width; ++col)
		{
			// If the current point is white, and the previous point was black, then we have found a new root.
			if (OcvUtilities::isPointWhite(_image, Point(col, row)) && !OcvUtilities::isPointWhite(_image, Point(col - 1, row)))
			{
				numberOfRootsInRows[row]++;
			}
		}

		if (numberOfRootsInRows[row] == 0)	// It should be safe to assume that once we have hit a row with no roots, there will be no more roots below that row.
			break;
	}

	if (!includeZeroes)
	{
		// Remove all zeroes from the list.
		numberOfRootsInRows.erase(
			remove_if(numberOfRootsInRows.begin(), numberOfRootsInRows.end(), [](const int numberOfRootsInRow) { return numberOfRootsInRow == 0; } ),
			numberOfRootsInRows.end()
			);
	}

	return numberOfRootsInRows;
}
