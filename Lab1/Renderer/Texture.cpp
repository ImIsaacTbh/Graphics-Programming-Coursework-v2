#include "Renderer/Texture.h"
#include "ThirdParty/stb/STB.h"
#include <cassert>
#include <iostream>

Texture::Texture(const std::string& fileName)
{
	int width = 0;
	int height = 0;
	int compnentCount = 0;
	unsigned char* imageData = stbi_load((fileName).c_str(), &width, &height, &compnentCount, 4); //4 is the required components (not important for us)
	_path = fileName;
	if (imageData == NULL)
	{
		std::cerr << "texture load failed" << fileName << std::endl;
	}
	Upload(width, height, imageData);
	stbi_image_free(imageData);
}

std::unique_ptr<Texture> Texture::CreateWhite()
{
	const unsigned char white[4] = { 255, 255, 255, 255 };

	std::unique_ptr<Texture> texture(new Texture());
	texture->Upload(1, 1, white);
	return texture;
}

void Texture::Upload(int width, int height, const unsigned char* rgba)
{
	glGenTextures(1, &textureHandler);
	glBindTexture(GL_TEXTURE_2D, textureHandler);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureHandler);
}

void Texture::Bind(unsigned int unit)
{
	assert(unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, textureHandler);
}
