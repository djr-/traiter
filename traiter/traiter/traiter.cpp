#include "general_utilities.h"
#include "root_system.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat originalImage;

int main(int argc, char** argv)
{
	if (argc < 2 || !GeneralUtilities::fileExists(argv[1]))
		return EXIT_FAILURE;

	originalImage = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

	RootSystem rootSystem = RootSystem(originalImage);

	cout << "Network area: " << rootSystem.networkArea() << " pixels.\n";
	cout << "Perimeter: " << rootSystem.perimeter() << " pixels.\n";
	cout << "Convex Area: " << rootSystem.convexArea() << " pixels.\n";
	cout << "Network Depth: " << rootSystem.networkDepth() << " pixels.\n";
	cout << "Netowrk Width: " << rootSystem.networkWidth() << " pixels.\n";
	cout << "Major Axis: " << rootSystem.majorAxis() << " pixels.\n";
	cout << "Minor Axis: " << rootSystem.minorAxis() << " pixels.\n";
	cout << "Aspect ratio: " <<  rootSystem.aspectRatio() << endl;
	cout << "Network solidity: " << rootSystem.networkSolidity() << endl;
	cout << "Network width to depth ratio: " << rootSystem.networkWidthToDepthRatio() << endl;
	cout << "Median number of roots: " << rootSystem.medianNumberOfRoots() << endl;

	imshow("Root System Image", rootSystem.getImage());

	waitKey();
	
	return EXIT_SUCCESS;
}
