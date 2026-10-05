#pragma once
#include <string>
#include <vector>

namespace FileSystem
{
	// Names (not paths) of the regular files directly inside directory.
	std::vector<std::string> ListFiles(const std::string& directory);

	// Lower-cased extension including the dot (".png"), or "" if there is none.
	std::string GetExtension(const std::string& fileName);

	// Full paths of every image file in directory.
	std::vector<std::string> FindImages(const std::string& directory);

	// Full paths, without extension, of every .vert/.frag pair in directory (the form Shader expects).
	std::vector<std::string> FindShaderPrograms(const std::string& directory);
}
