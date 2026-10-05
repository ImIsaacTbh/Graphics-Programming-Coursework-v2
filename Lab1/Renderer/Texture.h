#pragma once
#include <memory>
#include <string>
#include <GL/glew.h>

class Texture
{
public:
	Texture(const std::string& fileName);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	// 1x1 white texture, used when a material has no diffuse map.
	static std::unique_ptr<Texture> CreateWhite();

	std::string GetPath() const { return _path; }
	void Bind(unsigned int unit); // 32
	GLuint GetID() const { return textureHandler; }

private:
	Texture() = default;
	void Upload(int width, int height, const unsigned char* rgba);

	std::string _path;
	GLuint textureHandler = 0;
};
