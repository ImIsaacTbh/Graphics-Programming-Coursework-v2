#include "Renderer/Mesh.h"
#include "ThirdParty/objl/objLoader.h"

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

Mesh::Mesh(const std::string& objPath)
{
	objl::Loader loader;
	if (!loader.LoadFile(objPath) || loader.LoadedMeshes.empty())
	{
		std::cerr << "Failed to load model: " << objPath << std::endl;
		return;
	}
	Upload(loader.LoadedMeshes[0]);
}

Mesh::~Mesh()
{
	//cleanup
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(NUM_BUFFERS, vertexArrayBuffers);
}

void Mesh::Upload(const objl::Mesh& model)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

	positions.reserve(model.Vertices.size());
	texCoords.reserve(model.Vertices.size());
	normals.reserve(model.Vertices.size());

	for (size_t i = 0; i < model.Vertices.size(); ++i)
	{
		const auto& v = model.Vertices[i];
		positions.emplace_back(v.Position.X, v.Position.Y, v.Position.Z);
		texCoords.emplace_back(v.TextureCoordinate.X, -v.TextureCoordinate.Y);
		normals.emplace_back(v.Normal.X, v.Normal.Y, v.Normal.Z);
	}

	drawCount = static_cast<unsigned int>(model.Indices.size());

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);

	// Positions
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VERTEXBUFFER]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions[0]), positions.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords[0]), texCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), normals.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.Indices.size() * sizeof(model.Indices[0]), model.Indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Draw() const
{
	if (drawCount == 0) return;

	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
