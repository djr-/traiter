#pragma once

#include <vector>
#include <cmath>

namespace Utility
{
	//////////////////////////////////////////////////////////////////////////////////
	// GeneralUtilities
	//
	// Helper functions for common computations. This class will probably grow to be
	// unmaintainable if it exceeds a few functions, and at that point should be split
	// into MathUtilities, FileUtilities, etc.
	//////////////////////////////////////////////////////////////////////////////////
	class GeneralUtilities final
	{
	public:
		// File utilities

		static bool fileExists(const std::string& fileName);

		// Math utilities

		template<typename T> static double computeMedian(std::vector<T> list);
	};
}

//////////////////////////////////////////////////////////////////////////////////
// computeMedian()
//
// Computes the median of the specified list and returns the result as a double.
//////////////////////////////////////////////////////////////////////////////////
template<typename T> static double Utility::GeneralUtilities::computeMedian(std::vector<T> list)
{
	std::sort(list.begin(), list.end());

	if (list.size() % 2 != 0)
		return list[list.size() / 2];
	else
		return (list[(list.size() / 2) - 1] + list[list.size() / 2]) / 2.0;
}