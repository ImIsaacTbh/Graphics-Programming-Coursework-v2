#pragma once
#include <string>
#include <GL/glew.h>

struct aiScene;

// GPU geometry loaded from a model file via Assimp. Shading lives in Material.
class Mesh
{
public:
	explicit Mesh(const std::string& modelPath);
	~Mesh();

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	void Draw() const;

private:
	void Upload(const aiScene& scene);

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
