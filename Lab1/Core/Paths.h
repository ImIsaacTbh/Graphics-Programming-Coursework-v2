#pragma once
#include <string>

// All asset paths are resolved relative to the working directory, which is the
// project folder (Lab1/) when launched from Visual Studio.
namespace Paths
{
	constexpr const char* ResourceDir = "../Lab1/Resources/";

	inline std::string Resource(const std::string& fileName)
	{
		return ResourceDir + fileName;
	}
}
