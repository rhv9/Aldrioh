#pragma once
#include <glad/glad.h>

enum class VertexAttrib : int
{
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	Int,
	Int2,
	Int3,
	Int4,
};

struct VertexAttribSize
{
	uint32_t byteSize;
	uint32_t count;
};

static VertexAttribSize GetVertexAttribSize(VertexAttrib type)
{
	switch (type)
	{
	case VertexAttrib::Float:  return { 4, 1 };
	case VertexAttrib::Float2: return { 4, 2 };
	case VertexAttrib::Float3: return { 4, 3 };
	case VertexAttrib::Float4: return { 4, 4 };
	case VertexAttrib::Int:  return { 4, 1 };
	case VertexAttrib::Int2: return { 4, 2 };
	case VertexAttrib::Int3: return { 4, 3 };
	case VertexAttrib::Int4: return { 4, 4 };
	}
	ASSERT(false, "Unknown Vertex Type!");
	return { 0, 0 };
}

static GLenum GetOpenGLVertexType(VertexAttrib type)
{
	switch (type)
	{
	case VertexAttrib::Float:
	case VertexAttrib::Float2:
	case VertexAttrib::Float3:
	case VertexAttrib::Float4:
		return GL_FLOAT;
	default:
		ASSERT(false, "Undefined VertexType being used.");
	}
	return 0;
}

class BufferElement
{
public:
	BufferElement(const std::string& name, VertexAttrib type, bool normalized)
		: name(name), type(type), normalized(normalized) {
	}

	std::string name;
	VertexAttrib type;
	bool normalized;
};

class BufferLayout
{
public:
	BufferLayout() = default;
	BufferLayout(const std::vector<BufferElement>& elements) : elements(elements)
	{
		CalculateStride();
	}

	uint32_t CalculateStride();

	uint32_t stride = 0;
	std::vector<BufferElement> elements{};
};


class VertexBuffer
{
public:
	VertexBuffer(uint32_t size);
	VertexBuffer(float* vertices, uint32_t count);

	~VertexBuffer();

	void Bind() const;
	void UnBind() const;

	void SetLayout(const BufferLayout& layout) { this->layout = layout; }
	const BufferLayout& GetLayout() { return layout; }
	void SetData(const void* data, uint32_t size);

private:
	GLuint vbo = -1;
	BufferLayout layout;
};

class IndexBuffer
{
public:
	IndexBuffer(uint32_t* indices, uint32_t count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;

	uint32_t GetCount() const { return iboCount; }

private:
	GLuint ibo = -1;
	int iboCount = 0;
};
