#include "Core/FileSystem.h"
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#else
#include <dirent.h>
#endif

namespace FileSystem
{
	std::vector<std::string> ListFiles(const std::string& directory)
	{
		std::vector<std::string> results;

#ifdef _WIN32
		WIN32_FIND_DATAA findData;
		HANDLE hFind = FindFirstFileA((directory + "*").c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE) return results;
		do
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
			results.push_back(findData.cFileName);
		} while (FindNextFileA(hFind, &findData));
		FindClose(hFind);
#else
		DIR* dir = opendir(directory.c_str());
		if (!dir) return results;
		struct dirent* entry;
		while ((entry = readdir(dir)) != nullptr)
		{
			if (entry->d_type != DT_REG) continue;
			results.push_back(entry->d_name);
		}
		closedir(dir);
#endif
		return results;
	}

	std::string GetExtension(const std::string& fileName)
	{
		size_t dotPos = fileName.rfind('.');
		if (dotPos == std::string::npos) return "";

		std::string ext = fileName.substr(dotPos);
		std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return (char)std::tolower(c); });
		return ext;
	}

	std::vector<std::string> FindImages(const std::string& directory)
	{
		static const std::vector<std::string> imageExtensions = { ".png", ".jpg", ".jpeg", ".bmp", ".tga" };
		std::vector<std::string> results;

		for (const std::string& fileName : ListFiles(directory))
		{
			std::string ext = GetExtension(fileName);
			if (std::find(imageExtensions.begin(), imageExtensions.end(), ext) != imageExtensions.end())
				results.push_back(directory + fileName);
		}
		return results;
	}

	std::vector<std::string> FindShaderPrograms(const std::string& directory)
	{
		std::vector<std::string> results;

		for (const std::string& fileName : ListFiles(directory))
		{
			std::string ext = GetExtension(fileName);
			if (ext != ".vert" && ext != ".frag") continue;

			std::string base = directory + fileName.substr(0, fileName.size() - ext.size());
			if (std::find(results.begin(), results.end(), base) == results.end())
				results.push_back(base);
		}
		return results;
	}
}
