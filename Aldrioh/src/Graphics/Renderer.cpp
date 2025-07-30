#include <pch.h>
#include "Renderer.h"
#include <glad/glad.h>
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "SubTexture.h"

#include "RenderQueue.h"

#include "ShaderManager.h"
#include <Game/SpriteCollection.h>

struct RenderData
{
	std::unique_ptr<VertexArray> batchTextureVA;
	Shader* shaderBatchTexture = nullptr;

	// Batching Textures
	static const uint32_t MAX_DRAWS = 5000;
	static const uint32_t MAX_BATCH_VERTICES = MAX_DRAWS * 4;
	static const uint32_t MAX_BATCH_INDICES = MAX_DRAWS * 6;

	const glm::vec4 BatchQuadVertices[4] =
	{
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f }
	};

	BatchVertex* batchBasePtr = nullptr;
	BatchVertex* batchPtr = nullptr;
	uint32_t drawCount = 0;
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

	renderData.shaderBatchTexture = &ShaderManager::Get().GetShader(ShaderName::BATCH_TEXTURE);

	// Batching
	renderData.batchTextureVA = std::make_unique<VertexArray>();
	renderData.batchTextureVA->Bind();

	renderData.batchBasePtr = new BatchVertex[RenderData::MAX_BATCH_VERTICES];
	renderData.batchPtr = renderData.batchBasePtr;
	renderData.drawCount = 0;

	uint32_t* batchIndices = new uint32_t[RenderData::MAX_BATCH_INDICES];

	// Buffer
	std::vector<BufferElement> batchBufferElements = {
	{ "aPos", VertexAttrib::Float4, false},
	{ "aTexCoord", VertexAttrib::Float2, false},
	};
	std::shared_ptr<VertexBuffer> batchVertexBuffer = std::make_shared<VertexBuffer>(sizeof(BatchVertex) * RenderData::MAX_BATCH_VERTICES);
	batchVertexBuffer->SetLayout({ batchBufferElements });
	renderData.batchTextureVA->SetVertexBuffer(batchVertexBuffer);

	// Indices
	int offset = 0;
	for (int i = 0; i < RenderData::MAX_DRAWS; i += 6)
	{
		batchIndices[i + 0] = offset + 0;
		batchIndices[i + 1] = offset + 1;
		batchIndices[i + 2] = offset + 2;
		batchIndices[i + 3] = offset + 2;
		batchIndices[i + 4] = offset + 3;
		batchIndices[i + 5] = offset + 0;

		offset += 4;
	}
	std::shared_ptr<IndexBuffer> batchIndexBuffer = std::make_shared<IndexBuffer>(batchIndices, RenderData::MAX_BATCH_INDICES);
	delete[] batchIndices;
}

static glm::mat4 viewProjection;

void Renderer::StartScene(const Camera& camera)
{
	viewProjection = camera.GetProjection();

	// Batching
	renderData.shaderBatchTexture->Use();
	renderData.shaderBatchTexture->UniformMat4("u_ViewProjectionMatrix", camera.GetProjection());
	renderData.shaderBatchTexture->UniformInt("uTextureSampler", 0);

	//glClearColor(1.00f, 0.49f, 0.04f, 1.00f);
	glClearColor(0.20f, 0.29f, 0.84f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ResetBatch();
}

void Renderer::DrawQuad(const glm::vec3& position, const glm::vec2& scale)
{
	LOG_CORE_CRITICAL("NOT USING THIS FUNCTION RIGHT NOW!");
}

void Renderer::DrawQuad(const glm::vec3& position, const std::shared_ptr<Texture>& texture, const glm::vec2& scale)
{
	DrawQuad(position, texture.get(), scale);
}

void inline Renderer::SetBatchVertexBuffer(BatchVertex* ptr, const glm::vec4& pos, const glm::vec2& texCoords)
{
	ptr->pos = pos;
	ptr->texCoord = texCoords;
}


void Renderer::DrawQuad(const glm::vec3& position, const Texture* texture, const glm::vec2& scale)
{
	//glm::mat4 transform = glm::translate(glm::identity<glm::mat4>(), position) * glm::scale(glm::identity<glm::mat4>(), { scale, 1.0f });

	glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), position), { scale, 1.0f });

	const TextureCoords& texCoords = texture->GetTexCoords();
	const glm::vec2 texCoordsArray[4] =
	{
		{ texCoords.bottomLeft.x, texCoords.topRight.y   },
		{ texCoords.bottomLeft.x, texCoords.bottomLeft.y },
		{ texCoords.topRight.x,   texCoords.bottomLeft.y },
		{ texCoords.topRight.x,   texCoords.topRight.y   },
	};

	for (int i = 0; i < 4; ++i)
	{
		SetBatchVertexBuffer(renderData.batchPtr, transform * renderData.BatchQuadVertices[i], texCoordsArray[i]);
		++renderData.batchPtr;
	}

	renderData.drawCount++;
}

void Renderer::SetRenderDepthOnly(bool val) { renderState.renderDepth = val; }

bool Renderer::IsRenderDepth() { return renderState.renderDepth; }

void Renderer::EndScene()
{
	FlushBatch();
}

void Renderer::Destroy()
{
	delete[] renderData.batchBasePtr;
}

void Renderer::FlushBatch()
{
	if (renderData.drawCount == 0)
		return;

	uint32_t dataSize = static_cast<uint32_t>((uint8_t*)renderData.batchPtr - (uint8_t*)renderData.batchBasePtr);

	renderData.batchTextureVA->GetVertexBuffer()->SetData(renderData.batchBasePtr, dataSize);

	renderData.batchTextureVA->Bind();
	Sprites::get(Sprites::fire)->Bind(0);
	
	glDrawElements(GL_TRIANGLES, renderData.drawCount * 6, GL_UNSIGNED_INT, 0);
}

void Renderer::ResetBatch()
{
	renderData.batchPtr = renderData.batchBasePtr;
	renderData.drawCount = 0;
}

