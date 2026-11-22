#include "Renderer/Renderer.h"
#include "Core/Paths.h"
#include "Core/Window.h"
#include "Scene/Scene.h"
#include <algorithm>
#include <string>
#include <glm/gtc/matrix_transform.hpp>

static const glm::vec4 ClearColor(0.13f, 0.6f, 0.71f, 1.0f);
static const int ShadowMapFirstUnit = 3;

void Renderer::Init()
{
	_whiteTexture = Texture::CreateWhite();
	_shadowMaps.Init();
	_shadowShader = std::make_unique<Shader>(Paths::Resource("shadowDepth"));
}

void Renderer::Render(const Scene& scene, const Window& window)
{
	std::vector<glm::mat4> lightSpace = CalculateLightSpaceMatrices(scene.lights);
	// PASS 1: shadow maps
	_shadowMaps.Render(lightSpace, scene.GetObjects(), *_shadowShader);

	// PASS 2: lit scene to the screen
	glViewport(0, 0, window.GetWidth(), window.GetHeight());
	glCullFace(GL_BACK);
	glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawObjects(scene, lightSpace);
}

//being replaced rn delete later
// std::vector<glm::mat4> Renderer::CalculateLightSpaceMatrices(const Scene& scene)
// {
// 	std::vector<glm::mat4> matrices;
// 	for (const Light& light : scene.lights)
// 	{
// 		glm::mat4 lightView = glm::lookAt(light.position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
// 		glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 1000.0f);
// 		matrices.push_back(lightProjection * lightView);
// 	}
// 	return matrices;
// }

std::vector<glm::mat4> Renderer::CalculateLightSpaceMatrices(const std::vector<Light>& lights) {
	const glm::vec3 target(0.0f);
	std::vector<glm::mat4> matrices;
	for (const Light& light : lights) {
		glm::vec3 dir = glm::normalize(target - light.position);
		glm::vec3 up = std::abs(dir.y) > 0.99f ? glm::vec3(0, 0, 1) : glm::vec3(0, 1, 0);

		glm::mat4 view = glm::lookAt(light.position, target, up);
		glm::mat4 proj = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 1000.0f);
		matrices.push_back(view * proj);
	}
	return matrices;
}

void Renderer::DrawObjects(const Scene& scene, const std::vector<glm::mat4>& matrices)
{
	int shadowedLights = std::min((int)scene.lights.size(), MaxLights);

	for (const GameObject& obj : scene.GetObjects())
	{
		Shader* shader = obj.material.shader.get();
		if (shader == nullptr) continue;

		shader->Bind();
		shader->Update(obj.transform, scene.camera, obj.material.basicColor);
		shader->SetLights(scene.lights);
		shader->SetLightSpaceMatrices(matrices);

		const Texture* diffuse = obj.material.diffuse ? obj.material.diffuse.get() : _whiteTexture.get();
		shader->SetTexture("diffuse", diffuse->GetID(), 0);

		for (int l = 0; l < shadowedLights; l++)
		{
			shader->SetTexture("shadowMap[" + std::to_string(l) + "]", _shadowMaps.GetTexture(l), ShadowMapFirstUnit + l);
		}

		obj.mesh->Draw();
	}
}
