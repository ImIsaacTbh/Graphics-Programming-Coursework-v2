#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

// How a surface is shaded. Either resource may be null.
struct Material
{
	std::unique_ptr<Shader> shader;
	std::unique_ptr<Texture> diffuse;
	glm::vec3 basicColor = glm::vec3(0.0f);
};
