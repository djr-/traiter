#include <opencv2/core/core.hpp>

class RootSystem sealed
{
public:
	RootSystem(cv::Mat image);

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

	cv::Mat _image;
};
