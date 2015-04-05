#include "general_utilities.h"
#include <sys/stat.h>
#include <algorithm>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
// fileExists()
//
// Returns true if the specified file exists, false otherwise.
//////////////////////////////////////////////////////////////////////////////////
bool GeneralUtilities::fileExists(const string& fileName)
{
	struct stat buffer;
	return stat(fileName.c_str(), &buffer) == 0;
}
