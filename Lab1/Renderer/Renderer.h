#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "Renderer/ShadowMaps.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

class Scene;
class Window;

// Draws a Scene: shadow pass, then the lit pass to the default framebuffer.
class Renderer
{
public:
	// Requires a current GL context.
	void Init();
	void Render(const Scene& scene, const Window& window);

private:
	void DrawObjects(const Scene& scene);
	static std::vector<glm::mat4> CalculateLightSpaceMatrices(const Scene& scene);

	ShadowMaps _shadowMaps;
	std::unique_ptr<Shader> _shadowShader;
	std::unique_ptr<Texture> _whiteTexture;
};
