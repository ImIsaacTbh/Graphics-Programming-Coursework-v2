#include "Scene/Scene.h"
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
