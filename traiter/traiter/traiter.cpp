#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;

int main()
{
	Mat image = imread("roots.jpg", CV_LOAD_IMAGE_GRAYSCALE);	//TODO_DR: Replace with a try_read
	Mat thresholdImage = image.clone();
	Mat adaptiveThresholdImage = image.clone();
	
	threshold(image, thresholdImage, 120, 255, 3);
	adaptiveThreshold(image, adaptiveThresholdImage, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 0);
	imshow("original", image);
	imshow("threshold", thresholdImage);
	imshow("adaptive threshold", adaptiveThresholdImage);
	waitKey();
	
	return 0;
}