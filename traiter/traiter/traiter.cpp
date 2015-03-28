#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <sys/stat.h>
#include "root_system.h"

using namespace std;
using namespace cv;

bool fileExists(const string& fileName);

Mat originalImage;

int main(int argc, char** argv)
{
	if (argc < 2 || !fileExists(argv[1]))
		return EXIT_FAILURE;

	originalImage = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	RootSystem rootSystem = RootSystem(originalImage);

	cout << "Network area: " << rootSystem.networkArea() << " pixels.\n";
	cout << "Perimeter: " << rootSystem.perimeter() << " pixels.\n";
	rootSystem.aspectRatio();
	rootSystem.convexArea();
	rootSystem.networkDepth();
	rootSystem.networkWidth();

	imshow("Root System Image", rootSystem.getImage());

	waitKey();
	
	return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////
// fileExists()
//
// Returns true if the specified file exists, false otherwise.
//////////////////////////////////////////////////////////////////////////////////
bool fileExists(const string& fileName)
{
	struct stat buffer;
	return stat(fileName.c_str(), &buffer) == 0;
}
