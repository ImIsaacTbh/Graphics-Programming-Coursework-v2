#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Scene/Camera.h"
#include "Scene/GameObject.h"
#include "Scene/Primitives.h"
#include "Renderer/Light.h"

// Everything that exists in the world: objects, lights and the camera.
// Knows nothing about how it gets drawn or edited.
class Scene
{
public:
	// Loads meshPath and creates its material. An empty name becomes "Object <id>".
	GameObject& AddObject(const std::string& meshPath, const std::string& shaderPath, const std::string& texturePath,
		const Transform& transform, const std::string& name = "");

	GameObject& AddPrimitive(Primitive type, const std::string& shaderPath, const std::string& texturePath,
		const glm::vec3& scale = glm::vec3(1));

	void RemoveObject(size_t index);
	GameObject* FindById(int id);

	std::vector<GameObject>& GetObjects() { return _objects; }
	const std::vector<GameObject>& GetObjects() const { return _objects; }

	Camera camera;

	// Lights used for shading.
	std::vector<Light> lights;

	// Viewpoints the shadow maps are rendered from (one map per entry, up to MaxLights).
	// NOTE: these are kept separate from `lights` to preserve existing behaviour; they
	// do not currently line up with the lights the shaders sample shadows for.
	std::vector<glm::vec3> shadowCasterPositions;

private:
	int NextId() const;

	std::vector<GameObject> _objects;
};
