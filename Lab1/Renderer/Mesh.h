#pragma once
#include <string>
#include <GL/glew.h>

namespace objl { struct Mesh; }

// GPU geometry loaded from an .obj file. Shading lives in Material.
class Mesh
{
public:
	explicit Mesh(const std::string& objPath);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	void Draw() const;

private:
	void Upload(const objl::Mesh& model);

	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint vertexArrayObject = 0;
	GLuint vertexArrayBuffers[NUM_BUFFERS] = {};
	unsigned int drawCount = 0;
};
