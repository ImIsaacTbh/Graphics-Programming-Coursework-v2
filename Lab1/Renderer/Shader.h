#pragma once
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Renderer/Light.h"
#include "Scene/Transform.h"
#include "Scene/Camera.h"

// A linked vertex + fragment program loaded from <path>.vert / <path>.frag.
class Shader
{
public:
	Shader(const std::string& path);
	~Shader();

	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	std::string GetPath() const { return _path; }
	GLuint GetProgram() const { return program; }
	void Bind();

	// Per-object matrices, camera position and the primary light.
	// void Update(const Transform& transform, const Camera& camera, const glm::vec3& lightPos, const glm::mat4& lightSpaceMatrix, const glm::vec3& basicColor = glm::vec3(0, 0, 0));
	void Update(const Transform& transform, const Camera& camera, const glm::vec3& basicColor = glm::vec3(0));
	void SetLights(const std::vector<Light>& lights, const std::vector<int>& shadowSlots);
	void SetLightSpaceMatrices(const std::vector<glm::mat4>& matrices);
	void SetTexture(const std::string& name, GLuint textureID, int unit);
	void SetMat4(const std::string& name, const glm::mat4& value);

private:
	static std::string LoadShader(const std::string& fileName);
	static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	static GLuint CreateShader(const std::string& text, unsigned int type);

	std::string _path;
	GLuint program;

	static const unsigned int NUM_SHADERS = 2;
	enum
	{
		TRANSFORM_U,
		MODEL_U,
		AMBIENT_COLOR_U,
		BASIC_COLOUR_U,
		VIEW_POS_U,

		NUM_UNIFORMS
	};

	GLuint shaders[NUM_SHADERS];
	GLint uniforms[NUM_UNIFORMS];
};
