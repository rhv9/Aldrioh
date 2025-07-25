#include <pch.h>
#include "Renderer.h"
#include <glad/glad.h>
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "SubTexture.h"

#include "RenderQueue.h"

#include "ShaderManager.h"

struct RenderData
{
    VertexArray quadVA;
    Shader* shaderTexQuad;

    VertexArray quadTexCoordVA;
    Shader* shaderTexCoordQuad;
};

static struct RenderState
{
    bool renderDepth = false;
};

static RenderData renderData;
static RenderState renderState;

void Renderer::Init()
{
    LOG_CORE_INFO("Renderer::Init()");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1);
    //glDepthMask(GL_FALSE);    

    //renderData.shaderTexQuad = CreateRef<Shader>("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
    renderData.shaderTexQuad = &ShaderManager::Get().GetShader(ShaderName::OTHER_TEXTURE);
    renderData.shaderTexCoordQuad = &ShaderManager::Get().GetShader(ShaderName::GENERAL_TEXTURE);


    // Normal Quad
    float vertices[] = {
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0 
    };

    VertexDataMap vertexDatas = {
        { "aPos", 3, VertexDataType::Float, VertexDataBool::False},
        { "aTexCoord", 2, VertexDataType::Float, VertexDataBool::False},
    };

    renderData.quadVA = VertexArray::Create(vertexDatas, vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(unsigned int));
    
    // TextureTexCoord.glsl
    float verticesTexCoord[] = {
        0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 0.0f, 2.0f, 3.0f,
         1.0f, 0.0f, 0.0f, 4.0f, 5.0f,
         1.0f,  1.0f, 0.0f, 6.0f, 7.0f,
    };

    unsigned int indicesTexCoord[] = {
        0, 1, 2, 2, 3, 0
    };

    VertexDataMap vertexDatasTexCoord = {
        { "aPos", 3, VertexDataType::Float, VertexDataBool::False},
        { "aTexIndexCoords", 2, VertexDataType::Float, VertexDataBool::False},
    };

    renderData.quadTexCoordVA = VertexArray::Create(vertexDatasTexCoord, verticesTexCoord, sizeof(verticesTexCoord) / sizeof(float), indicesTexCoord, sizeof(indicesTexCoord) / sizeof(unsigned int));

}

static glm::mat4 viewProjection;

void Renderer::StartScene(const Camera& camera)
{
    viewProjection = camera.GetProjection();

    renderData.shaderTexQuad->Use();
    renderData.shaderTexQuad->UniformMat4("u_ViewProjectionMatrix", camera.GetProjection());
    renderData.shaderTexQuad->UniformInt("uRenderDepth", renderState.renderDepth);
    renderData.shaderTexCoordQuad->Use();
    renderData.shaderTexCoordQuad->UniformInt("uRenderDepth", renderState.renderDepth);
    renderData.shaderTexCoordQuad->UniformMat4("u_ViewProjectionMatrix", camera.GetProjection());

    //glClearColor(1.00f, 0.49f, 0.04f, 1.00f);
    glClearColor(0.20f, 0.29f, 0.84f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& scale)
{
    renderData.shaderTexQuad->Use();

    glm::mat4 transform = glm::scale(glm::identity<glm::mat4>(), { scale, 1.0f }) * glm::translate(glm::identity<glm::mat4>(), position);


    renderData.shaderTexQuad->UniformMat4("u_Transform", transform);
    renderData.shaderTexQuad->UniformInt("uTextureSampler", 0);

    renderData.quadVA.Bind();

	glDrawElements(GL_TRIANGLES, renderData.quadVA.GetIndicesCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::DrawQuad(const glm::vec3& position, const std::shared_ptr<Texture>& texture, const glm::vec2& scale)
{
    DrawQuad(position, texture.get(), scale);
}

void Renderer::DrawQuad(const glm::vec3& position, const Texture* texture, const glm::vec2& scale)
{
    renderData.shaderTexCoordQuad->Use();

    glm::mat4 transform = glm::translate(glm::identity<glm::mat4>(), position) * glm::scale(glm::identity<glm::mat4>(), { scale, 1.0f });

    renderData.shaderTexCoordQuad->UniformMat4("u_Transform", transform);
    renderData.shaderTexCoordQuad->UniformInt("uTextureSampler", 0);

    const TextureCoords& texCoords = texture->GetTexCoords();

    float texCoordsArray[8] =
    {
        texCoords.bottomLeft.x, texCoords.topRight.y,
        texCoords.bottomLeft.x, texCoords.bottomLeft.y,
        texCoords.topRight.x, texCoords.bottomLeft.y,
        texCoords.topRight.x, texCoords.topRight.y,
    };


    renderData.shaderTexCoordQuad->UniformFloatArray("uTexCoords", texCoordsArray, 8);

    renderData.quadTexCoordVA.Bind();

    texture->Bind(0);
    glDrawElements(GL_TRIANGLES, renderData.quadTexCoordVA.GetIndicesCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::SetRenderDepthOnly(bool val) { renderState.renderDepth = val; }

bool Renderer::IsRenderDepth() { return renderState.renderDepth; }

void Renderer::EndScene()
{
}