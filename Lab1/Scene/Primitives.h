#pragma once
#include <string>

// Built-in meshes that can be spawned from the editor.
enum class Primitive
{
	Cube,
	IcoSphere,
	UVSphere,
	Plane,
	Monki,
	Lucio
};

struct PrimitiveInfo
{
	std::string meshPath;
	const char* displayName;
};

PrimitiveInfo GetPrimitiveInfo(Primitive type);
