#pragma once
#include <string>

namespace Paths
{
#ifdef GP2_RESOURCE_DIR
	// Absolute path, set by CMake.
	constexpr const char* ResourceDir = GP2_RESOURCE_DIR;
#else
	// Relative to the working directory, which is the project folder (Lab1/)
	// when launched from Visual Studio.
	constexpr const char* ResourceDir = "../Lab1/Resources/";
#endif

	inline std::string Resource(const std::string& fileName)
	{
		return ResourceDir + fileName;
	}
}
