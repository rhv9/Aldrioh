#include "pch.h"
#include "VertexArray.h"


static int sizeofVertexDataType(VertexDataType vertexDataType)
{
    switch (vertexDataType)
    {
    case VertexDataType::Float: return sizeof(float);
    }

    ASSERT("Not implemented vertex data type");
    return -1;
}

VertexArray VertexArray::Create(VertexDataMap vertexDatas, float* vertices, uint32_t verticesCount, uint32_t* indices, uint32_t indicesCount)
{
    VertexArray vertexArray;

    glGenVertexArrays(1, &vertexArray.vao);
    glBindVertexArray(vertexArray.vao);

    glGenBuffers(1, &vertexArray.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertexArray.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesCount, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &vertexArray.ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArray.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesCount, indices, GL_STATIC_DRAW);
    vertexArray.iboCount = indicesCount;

    // Calculate stride
    int stride = 0;
    for (const VertexData& vertexData : vertexDatas)
    {
        stride += sizeofVertexDataType(vertexData.type) * vertexData.size;
    }

    int pointer = 0;
    for (int i = 0; i < vertexDatas.size(); ++i)
    {
        const VertexData& vertexData = vertexDatas[i];
        glVertexAttribPointer(i, vertexData.size, vertexData.type, vertexData.normalized, stride, (void*)pointer);
        glEnableVertexAttribArray(i);

        pointer += sizeofVertexDataType(vertexData.type) * vertexData.size;
    }

    return vertexArray;
}

void VertexArray::Bind() const
{
    glBindVertexArray(vao);
}
