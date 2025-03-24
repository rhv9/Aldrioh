#pragma once

#include <glad/glad.h>

enum VertexDataType : int
{
	Float = GL_FLOAT,
};

enum VertexDataBool : int
{
	True = GL_TRUE,
	False = GL_FALSE,
};


struct VertexData
{
	std::string name;
	uint32_t size;
	VertexDataType type;
	VertexDataBool normalized;
};

using VertexDataMap = std::vector<VertexData>;

class VertexArray
{
public:
	VertexArray() {}

	void Bind() const;

	int GetIndicesCount() const { return iboCount; }

	static VertexArray Create(VertexDataMap vertexDatas, float* vertices, uint32_t verticesCount, uint32_t* indices, uint32_t indicesCount);

private:
	GLuint vao = -1, vbo = -1, ibo = -1;
	int iboCount = 0;
};

