#pragma once
#include <memory>
#include <string>
#include <cstdio>
#include "Scene/Transform.h"
#include "Renderer/Mesh.h"
#include "Renderer/Material.h"

// A renderable entity in the scene: geometry + how to shade it + where it is.
struct GameObject
{
	GameObject(int id, const std::string& displayName, const Transform& transform, std::unique_ptr<Mesh> mesh, Material material)
		: id(id), transform(transform), mesh(std::move(mesh)), material(std::move(material))
	{
		snprintf(name, sizeof(name), "%s", displayName.c_str());
	}

	int id;
	char name[64]; // fixed buffer so the editor can edit it in place
	Transform transform;
	std::unique_ptr<Mesh> mesh;
	Material material;
};
