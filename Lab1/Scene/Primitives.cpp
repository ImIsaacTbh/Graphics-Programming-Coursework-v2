#include "Scene/Primitives.h"
#include "Core/Paths.h"

PrimitiveInfo GetPrimitiveInfo(Primitive type)
{
	switch (type)
	{
	case Primitive::Cube:		return { Paths::Resource("cube.obj"),		"Cube" };
	case Primitive::IcoSphere:	return { Paths::Resource("smolSPHERE.obj"),	"ICOSphere" };
	case Primitive::UVSphere:	return { Paths::Resource("uvsphere.obj"),	"UVSphere" };
	case Primitive::Plane:		return { Paths::Resource("plane.obj"),		"Plane" };
	case Primitive::Monki:		return { Paths::Resource("monki.obj"),		"Monki" };
	case Primitive::Lucio:		return { Paths::Resource("lucio.obj"),		"Lucio" };
	default:					return { Paths::Resource("cube.obj"),		"Cube" };
	}
}
