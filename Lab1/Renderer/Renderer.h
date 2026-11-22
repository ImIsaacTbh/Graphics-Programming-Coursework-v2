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
	struct FrameLights {
		std::vector<Light> shaded; // enabled lights
		std::vector<int> shadowSlot; // on per shaded light
		std::vector<glm::mat4> lightSpace; // one per used shadow slot
	};
	static FrameLights GatherLights(const std::vector<Light>& lights);
	static glm::mat4 LightSpaceMatrix(const Light& light);

	void DrawObjects(const Scene& scene, const FrameLights& frame);
	static std::vector<glm::mat4> CalculateLightSpaceMatrices(const std::vector<Light>&lights);

	ShadowMaps _shadowMaps;
	std::unique_ptr<Shader> _shadowShader;
	std::unique_ptr<Texture> _whiteTexture;
};
