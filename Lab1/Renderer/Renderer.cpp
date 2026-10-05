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
	// PASS 1: shadow maps
	_shadowMaps.Render(scene.shadowCasterPositions, scene.GetObjects(), *_shadowShader);

	// PASS 2: lit scene to the screen
	glViewport(0, 0, window.GetWidth(), window.GetHeight());
	glCullFace(GL_BACK);
	glClearColor(ClearColor.r, ClearColor.g, ClearColor.b, ClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawObjects(scene);
}

std::vector<glm::mat4> Renderer::CalculateLightSpaceMatrices(const Scene& scene)
{
	std::vector<glm::mat4> matrices;
	for (const Light& light : scene.lights)
	{
		glm::mat4 lightView = glm::lookAt(light.position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 1000.0f);
		matrices.push_back(lightProjection * lightView);
	}
	return matrices;
}

void Renderer::DrawObjects(const Scene& scene)
{
	if (scene.lights.empty()) return;

	std::vector<glm::mat4> lightSpaceMatrices = CalculateLightSpaceMatrices(scene);
	int shadowedLights = std::min((int)scene.lights.size(), MaxLights);

	for (const GameObject& obj : scene.GetObjects())
	{
		Shader* shader = obj.material.shader.get();
		if (shader == nullptr) continue;

		shader->Bind();
		shader->Update(obj.transform, scene.camera, scene.lights[0].position, lightSpaceMatrices[0], obj.material.basicColor);
		shader->SetLights(scene.lights);
		shader->SetLightSpaceMatrices(lightSpaceMatrices);

		const Texture* diffuse = obj.material.diffuse ? obj.material.diffuse.get() : _whiteTexture.get();
		shader->SetTexture("diffuse", diffuse->GetID(), 0);

		for (int l = 0; l < shadowedLights; l++)
		{
			shader->SetTexture("shadowMap[" + std::to_string(l) + "]", _shadowMaps.GetTexture(l), ShadowMapFirstUnit + l);
		}

		obj.mesh->Draw();
	}
}
