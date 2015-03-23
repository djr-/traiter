#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <sys/stat.h>
#include "root_system.h"

using namespace cv;

bool fileExists(const string& fileName);
void onThresholdTrackbar(int thresholdValue, void*);
void onAdaptiveThresholdTrackbar(int blockSize, void*);

const int maximumThresholdValue = 255;
const int maximumBlockSize = 255;
const string TRACKBAR_NAME = "Threshold";
const string THRESHOLD_WINDOW_NAME = "Threshold";
const string ADAPTIVE_THRESHOLD_WINDOW_NAME = "Adaptive Threshold";

int thresholdType = THRESH_BINARY;
Mat originalImage;

int main(int argc, char** argv)
{
	if (argc < 2 || !fileExists(argv[1]))
		return EXIT_FAILURE;

	originalImage = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	namedWindow(THRESHOLD_WINDOW_NAME);
	namedWindow(ADAPTIVE_THRESHOLD_WINDOW_NAME);

	int thresholdValue = 0;
	createTrackbar(TRACKBAR_NAME, THRESHOLD_WINDOW_NAME, &thresholdValue, maximumThresholdValue, onThresholdTrackbar);
	onThresholdTrackbar(thresholdValue, nullptr);

	int blockSize = 0;
	createTrackbar(TRACKBAR_NAME, ADAPTIVE_THRESHOLD_WINDOW_NAME, &blockSize, maximumBlockSize, onAdaptiveThresholdTrackbar);
	onAdaptiveThresholdTrackbar(blockSize, nullptr);

	waitKey();
	
	return EXIT_SUCCESS;
}

bool fileExists(const string& fileName)
{
	struct stat buffer;
	return stat(fileName.c_str(), &buffer) == 0;
}

void onThresholdTrackbar(int thresholdValue, void*)
{
	Mat thresholdImage = originalImage.clone();
	threshold(originalImage, thresholdImage, thresholdValue, maximumThresholdValue, thresholdType);
	imshow(THRESHOLD_WINDOW_NAME, thresholdImage);
}

void onAdaptiveThresholdTrackbar(int blockSize, void*)
{
	Mat adaptiveThresholdImage = originalImage.clone();
	if (blockSize < 3)
		blockSize = 3;
	if (blockSize % 2 == 0)
		blockSize++;
	int c = 0;
	adaptiveThreshold(originalImage, adaptiveThresholdImage, maximumBlockSize, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, blockSize, c);
	imshow(ADAPTIVE_THRESHOLD_WINDOW_NAME, adaptiveThresholdImage);
}
