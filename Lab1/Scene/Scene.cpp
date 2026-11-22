#include "Scene/Scene.h"

#include <algorithm>
#include <memory>

int Scene::NextId() const
{
	return _objects.empty() ? 0 : _objects.back().id + 1;
}

GameObject& Scene::AddObject(const std::string& meshPath, const std::string& shaderPath, const std::string& texturePath,
	const Transform& transform, const std::string& name)
{
	int id = NextId();

	Material material;
	if (!shaderPath.empty())
		material.shader = std::make_unique<Shader>(shaderPath);
	if (!texturePath.empty())
		material.diffuse = std::make_unique<Texture>(texturePath);

	std::string displayName = name.empty() ? "Object " + std::to_string(id) : name;

	_objects.emplace_back(id, displayName, transform, std::make_unique<Mesh>(meshPath), std::move(material));
	return _objects.back();
}

GameObject& Scene::AddPrimitive(Primitive type, const std::string& shaderPath, const std::string& texturePath, const glm::vec3& scale)
{
	PrimitiveInfo info = GetPrimitiveInfo(type);
	std::string name = std::string(info.displayName) + " " + std::to_string(NextId());

	return AddObject(info.meshPath, shaderPath, texturePath, Transform(glm::vec3(0), glm::vec3(0), scale), name);
}

void Scene::RemoveObject(size_t index)
{
	if (index < _objects.size())
		_objects.erase(_objects.begin() + index);
}

GameObject* Scene::FindById(int id)
{
	for (GameObject& obj : _objects)
	{
		if (obj.id == id) return &obj;
	}
	return nullptr;
}

Light& Scene::AddLight(const glm::vec3& position, const glm::vec3& color, const std::string& name) {
	Light light;
	light.id = _nextLightId++;
	light.position = position;
	light.color = color;
	std::string displayName = name.empty() ? "Light " + std::to_string(light.id) : name;
	snprintf(light.name, sizeof(light.name), "%s", displayName.c_str());

	_lights.push_back(light);
	return _lights.back();
}

void Scene::RemoveLight(int id) {
	_lights.erase(std::remove_if(_lights.begin(), _lights.end(), [id](const Light& l) { return l.id == id; }), _lights.end());
}

Light* Scene::FindLightById(int id) {
	for(Light& l : _lights) {
		if (l.id == id) return &l;
	}
	return nullptr;
}
