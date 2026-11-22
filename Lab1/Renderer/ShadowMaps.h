#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Renderer/Light.h"

class Shader;
struct GameObject;

// One depth texture + framebuffer per light (up to MaxLights).
class ShadowMaps
{
public:
	static const int Width = 2048;
	static const int Height = 2048;

	ShadowMaps() = default;
	~ShadowMaps();

	ShadowMaps(const ShadowMaps&) = delete;
	ShadowMaps& operator=(const ShadowMaps&) = delete;

	void Init();

	// Renders scene depth from each position into its own map. Leaves the default framebuffer bound.
	void Render(const std::vector<glm::mat4>& lightSpaceMatrices, const std::vector<GameObject>& objects, Shader& depthShader);

	GLuint GetTexture(int index) const { return _textures[index]; }

private:
	bool _initialized = false;
	GLuint _textures[MaxLights] = {};
	GLuint _framebuffers[MaxLights] = {};
};
