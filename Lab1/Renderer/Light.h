#pragma once
#include <glm/glm.hpp>

// Must match MAX_LIGHTS in the lit shaders.
constexpr int MaxLights = 4;

struct Light
{
	glm::vec3 position;
	glm::vec3 color;
};
