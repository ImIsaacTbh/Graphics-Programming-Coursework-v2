#include "Renderer/ShadowMaps.h"
#include "Renderer/Shader.h"
#include "Scene/GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

ShadowMaps::~ShadowMaps()
{
	if (!_initialized) return;
	glDeleteFramebuffers(MaxLights, _framebuffers);
	glDeleteTextures(MaxLights, _textures);
}

void ShadowMaps::Init()
{
	for (int i = 0; i < MaxLights; i++)
	{
		glGenFramebuffers(1, &_framebuffers[i]);
		glGenTextures(1, &_textures[i]);
		glBindTexture(GL_TEXTURE_2D, _textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _textures[i], 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	_initialized = true;
}

void ShadowMaps::Render(const std::vector<glm::vec3>& casterPositions, const std::vector<GameObject>& objects, Shader& depthShader)
{
	depthShader.Bind();

	for (int l = 0; l < (int)casterPositions.size() && l < MaxLights; l++)
	{
		glm::mat4 lightView = glm::lookAt(casterPositions[l], glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, 100.0f, -100.0f, 0.1f, 1000.0f);
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		glBindFramebuffer(GL_FRAMEBUFFER, _framebuffers[l]);
		glViewport(0, 0, Width, Height);
		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);
		glEnable(GL_DEPTH_TEST);

		for (const GameObject& obj : objects)
		{
			depthShader.SetMat4("lightSpaceMVP", lightSpaceMatrix * obj.transform.GetModel());
			obj.mesh->Draw();
		}
	}
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
