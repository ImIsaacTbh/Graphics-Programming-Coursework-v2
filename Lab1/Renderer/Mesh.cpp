#include "Renderer/Mesh.h"

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

Mesh::Mesh(const std::string& modelPath)
{
	Assimp::Importer importer;
	// c_str(): Debug builds can't pass std::string across the prebuilt (Release) Windows DLL
	const aiScene* scene = importer.ReadFile(modelPath.c_str(),
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals);
	if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || scene->mNumMeshes == 0)
	{
		std::cerr << "Failed to load model: " << modelPath << " (" << importer.GetErrorString() << ")" << std::endl;
		return;
	}
	Upload(*scene);
}

Mesh::~Mesh()
{
	//cleanup
	glDeleteVertexArrays(1, &vertexArrayObject);
	glDeleteBuffers(NUM_BUFFERS, vertexArrayBuffers);
}

void Mesh::Upload(const aiScene& scene)
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	// Merge every sub-mesh into one set of buffers
	for (unsigned int m = 0; m < scene.mNumMeshes; ++m)
	{
		const aiMesh& mesh = *scene.mMeshes[m];
		const unsigned int baseVertex = static_cast<unsigned int>(positions.size());

		for (unsigned int i = 0; i < mesh.mNumVertices; ++i)
		{
			const aiVector3D& p = mesh.mVertices[i];
			positions.emplace_back(p.x, p.y, p.z);

			if (mesh.HasTextureCoords(0))
				texCoords.emplace_back(mesh.mTextureCoords[0][i].x, -mesh.mTextureCoords[0][i].y);
			else
				texCoords.emplace_back(0.0f, 0.0f);

			if (mesh.HasNormals())
				normals.emplace_back(mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z);
			else
				normals.emplace_back(0.0f, 0.0f, 0.0f);
		}

		for (unsigned int f = 0; f < mesh.mNumFaces; ++f)
		{
			const aiFace& face = mesh.mFaces[f];
			if (face.mNumIndices != 3) continue; // skip points/lines
			for (unsigned int k = 0; k < 3; ++k)
				indices.push_back(baseVertex + face.mIndices[k]);
		}
	}

	drawCount = static_cast<unsigned int>(indices.size());

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Draw() const
{
	if (drawCount == 0) return;

	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
