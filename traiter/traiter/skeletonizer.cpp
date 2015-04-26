#include "skeletonizer.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "direction.h"
#include "ocv_utilities.h"

using namespace cv;
using namespace morph;
using namespace traiter;
using namespace utility;

//////////////////////////////////////////////////////////////////////////////////
// computeMorphologicalSkeleton()
//
// Compute the morphological skeleton of the image. This algorithm is based on
// the skeletonization method described in Digital Image Processing by
// Gonzalez and Woods 3rd Edition (page 651-654).
//////////////////////////////////////////////////////////////////////////////////
Mat Skeletonizer::computeMorphologicalSkeleton(const Mat& originalImage)
{
	Mat image = originalImage.clone();
	Mat skeleton = Mat(image.size(), CV_8UC1, Scalar(0));
	Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
	Mat erodedImage;
	Mat openedImage;
	Mat temp;

	do
	{
		erode(image, erodedImage, element);
		dilate(erodedImage, openedImage, element);
		subtract(image, openedImage, temp);
		bitwise_or(skeleton, temp, skeleton);
		erodedImage.copyTo(image);

	} while (!countNonZero(openedImage) == 0);

	return skeleton;
}

//////////////////////////////////////////////////////////////////////////////////
// computeMedialAxisTransform()
//
// Computes the medial axis transform of the image.
//////////////////////////////////////////////////////////////////////////////////
Mat Skeletonizer::computeMedialAxisTransform(const Mat& originalImage)
{
	Mat image = originalImage.clone();
	//Mat skeleton = Mat(image.size(), CV_8UC1, Scalar(0));
	Mat skeleton = image.clone();

	//////////
	// For each point in the image,
	//		Find the distance to the nearest black point in every direction.
	//      If the minimum of this list only appears once, change the corresponding pixel in the skeleton image to black.
	//////////

	for (int col = 0; col < image.size().width; ++col)
	{
		for (int row = 0; row < image.size().height; ++row)
		{
			Point currentPoint = Point(col, row);

			if (OcvUtilities::isPointWhite(image, currentPoint))
			{
				int northDistance = OcvUtilities::distanceToNearestPointWithColor(image, currentPoint, NORTH, 0);
				int southDistance = OcvUtilities::distanceToNearestPointWithColor(image, currentPoint, SOUTH, 0);
				int eastDistance = OcvUtilities::distanceToNearestPointWithColor(image, currentPoint, EAST, 0);
				int westDistance = OcvUtilities::distanceToNearestPointWithColor(image, currentPoint, WEST, 0);

				//int northWestDistance = OcvUtilities::distanceToNearestPointWithColor(image, currentPoint, NORTHWEST, 0);
				//int southEastDistance = OcvUtilities::distanceToNearestPointWithColor(image, currentPoint, SOUTHEAST, 0);
				//int northEastDistance = OcvUtilities::distanceToNearestPointWithColor(image, currentPoint, NORTHEAST, 0);
				//int southWestDistance = OcvUtilities::distanceToNearestPointWithColor(image, currentPoint, SOUTHWEST, 0);

				vector<int> distances;
				distances.push_back(northDistance);
				distances.push_back(southDistance);
				distances.push_back(eastDistance);
				distances.push_back(westDistance);
				//distances.push_back(northWestDistance);
				//distances.push_back(southEastDistance);
				//distances.push_back(northEastDistance);
				//distances.push_back(southWestDistance);
				sort(distances.begin(), distances.end());
				//bool tmp = abs(distances[0] - distances[1]) == 1;
				if (distances[0] != distances[1])
				{
					// We have only one smallest distance to the nearest black pixel, so this isn't part of the MAT.
					skeleton.at<uchar>(currentPoint) = 0;
				}
			}
		}
	}

	return skeleton;
}