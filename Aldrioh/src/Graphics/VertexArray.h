#pragma once

#include <glad/glad.h>
#include "Buffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;

	void SetVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer);
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer);

	const std::shared_ptr<VertexBuffer> GetVertexBuffer() const { return vertexBuffer; }
	const std::shared_ptr<IndexBuffer> GetIndexBuffer() const { return indexBuffer; }


private:
	GLuint vao = -1;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer> indexBuffer;
};

