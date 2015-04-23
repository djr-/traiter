#pragma once

namespace traiter
{
	//////////////////////////////////////////////////////////////////////////////////
	// ThreshMethod
	//
	// Represents which type of thresholding should be performed.
	//////////////////////////////////////////////////////////////////////////////////
	enum ThreshMethod
	{
		// Basic thresholding
		THRESH,

		// Adaptive thresholding
		ADAPTIVE_THRESH,

		// Doubly adaptive thresholding
		DOUBLE_ADAPTIVE_THRESH
	};
}
