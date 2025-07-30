#include "pch.h"
#include "VertexArray.h"


VertexArray::VertexArray()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &vao);
}

void VertexArray::Bind() const
{
    glBindVertexArray(vao);
}

void VertexArray::SetVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer)
{
    LOG_CORE_TRACE("Setting VBO to VAO id: {}", vao);

    this->vertexBuffer = vertexBuffer;
    Bind();
    vertexBuffer->Bind();

    const BufferLayout& layout = vertexBuffer->GetLayout();
    int pointer = 0;
    for (int i = 0; i < layout.elements.size(); ++i)
    {
        const BufferElement& element = layout.elements[i];
        VertexAttribSize vertexAttribSize = GetVertexAttribSize(element.type);

        LOG_CORE_TRACE("Vertex Attrib {}, {}, {}, {}, {}, {}, {}", element.name, i, vertexAttribSize.count, GetOpenGLVertexType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.stride, (void*)pointer);


        glVertexAttribPointer(i, vertexAttribSize.count, GetOpenGLVertexType(element.type), element.normalized ? GL_TRUE : GL_FALSE, layout.stride, (void*)pointer);
        glEnableVertexAttribArray(i);

        pointer += vertexAttribSize.byteSize * vertexAttribSize.count;
    }

}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer)
{
    this->indexBuffer = indexBuffer;
    Bind();
    indexBuffer->Bind();
}
