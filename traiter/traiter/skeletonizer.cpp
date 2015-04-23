#include "skeletonizer.h"
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace morph;

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