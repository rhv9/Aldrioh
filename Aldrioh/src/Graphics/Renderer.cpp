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

#include <Debug/Statistics.h>
#include <Core/Platform.h>

#include <Game.h>
#include <Scene/CameraController.h>

#include <imgui.h>

struct BatchVertex
{
	glm::vec4 pos{ 0 };
	glm::vec2 texCoord{ 0 };
};

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

static RenderData renderData;

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
	for (int i = 0; i < RenderData::MAX_BATCH_INDICES; i += 6)
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
	renderData.batchTextureVA->SetIndexBuffer(batchIndexBuffer);

	delete[] batchIndices;

	InitUIRenderer();
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
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.20f, 0.29f, 0.84f, 1.00f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ResetBatch();
}

void Renderer::OnResize(WindowResizeEventArg& e)
{
	UIOnResize(e);
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
	if (renderData.drawCount >= RenderData::MAX_DRAWS)
		FlushAndReset();

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

void Renderer::EndScene()
{
	FlushBatch();
}

void Renderer::Destroy()
{
	delete[] renderData.batchBasePtr;
	DestroyUIRenderer();
}

void Renderer::FlushBatch()
{
	auto& renderStats = Statistics::RendererStats::GetUnderlyingStats();
	renderStats.batchQuads += renderData.drawCount;
	renderStats.batchVertices += renderData.drawCount * 4;
	renderStats.batchIndices += renderData.drawCount * 6;
	renderStats.drawCalls++;

	if (renderData.drawCount == 0)
		return;

	uint32_t dataSize = static_cast<uint32_t>((uint8_t*)renderData.batchPtr - (uint8_t*)renderData.batchBasePtr);
	//uint32_t dataSize = (renderData.drawCount) * 4 * sizeof(BatchVertex);

	renderData.batchTextureVA->Bind();
	renderData.batchTextureVA->GetVertexBuffer()->SetData(renderData.batchBasePtr, dataSize);

	Sprites::get(Sprites::fire)->Bind(0);

	glDrawElements(GL_TRIANGLES, renderData.drawCount * 6, GL_UNSIGNED_INT, 0);
}

void Renderer::FlushAndReset()
{
	FlushBatch();
	ResetBatch();
}


void Renderer::ResetBatch()
{
	renderData.batchPtr = renderData.batchBasePtr;
	renderData.drawCount = 0;
}


//
// UI 
//

struct UIVertex
{
	glm::vec4 pos;
	glm::vec2 texCoords;
	glm::vec4 colour;
	float flags;
};

struct UIRenderData
{
	Shader* shader = nullptr;
	std::unique_ptr<VertexArray> vao;

	CameraController* cameraController;

	// Batching Textures
	static const uint32_t MAX_DRAWS = 1000;
	static const uint32_t MAX_BATCH_VERTICES = MAX_DRAWS * 4;
	static const uint32_t MAX_BATCH_INDICES = MAX_DRAWS * 6;

	static constexpr float VERTEX_DEPTH_VALUE = 0.9f;

	const glm::vec4 BatchQuadVertices[4] =
	{
		{ 0.0f, 1.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 0.0f, 1.0f }
	};
	UIVertex* batchBasePtr = nullptr;
	UIVertex* batchPtr = nullptr;
	uint32_t drawCount = 0;
};

static UIRenderData* uiRd;

void Renderer::InitUIRenderer()
{
	LOG_CORE_INFO("Initializing UI Renderer");

	uiRd = new UIRenderData();
	float aspectRatio = Game::Instance().GetWindow()->GetAspectRatio();
	uiRd->cameraController = new CameraController(aspectRatio, 1);
	uiRd->shader = &ShaderManager::Get().GetShader(ShaderName::UI_SHADER);


	// Batching
	uiRd->vao = std::make_unique<VertexArray>();
	uiRd->vao->Bind();

	uiRd->batchBasePtr = new UIVertex[UIRenderData::MAX_BATCH_VERTICES];
	uiRd->batchPtr = uiRd->batchBasePtr;
	uiRd->drawCount = 0;

	uint32_t* batchIndices = new uint32_t[UIRenderData::MAX_BATCH_INDICES];

	// Buffer
	std::vector<BufferElement> batchBufferElements = {
	{ "aPos", VertexAttrib::Float4, false},
	{ "aTexCoord", VertexAttrib::Float2, false},
	{ "aColour", VertexAttrib::Float4, false},
	{ "aFlags", VertexAttrib::Float, false},
	};
	std::shared_ptr<VertexBuffer> batchVertexBuffer = std::make_shared<VertexBuffer>(sizeof(UIVertex) * UIRenderData::MAX_BATCH_VERTICES);
	batchVertexBuffer->SetLayout({ batchBufferElements });
	uiRd->vao->SetVertexBuffer(batchVertexBuffer);

	// Indices
	int offset = 0;
	for (int i = 0; i < UIRenderData::MAX_BATCH_INDICES; i += 6)
	{
		batchIndices[i + 0] = offset + 0;
		batchIndices[i + 1] = offset + 1;
		batchIndices[i + 2] = offset + 2;
		batchIndices[i + 3] = offset + 2;
		batchIndices[i + 4] = offset + 3;
		batchIndices[i + 5] = offset + 0;

		offset += 4;
	}
	std::shared_ptr<IndexBuffer> batchIndexBuffer = std::make_shared<IndexBuffer>(batchIndices, UIRenderData::MAX_BATCH_INDICES);
	uiRd->vao->SetIndexBuffer(batchIndexBuffer);

	delete[] batchIndices;
}

void Renderer::DestroyUIRenderer()
{
	LOG_CORE_INFO("Destroying UI Renderer");

	delete uiRd->cameraController;
	delete uiRd;
}

static glm::vec2 cameraPos{ 0 };
static float cameraZoom = 1;

void Renderer::UIOnResize(WindowResizeEventArg& e)
{
	uiRd->cameraController->OnResize(e.Width, e.Height);
	cameraZoom = e.Height / 2.0f;
	cameraPos.x = uiRd->cameraController->GetAspectRatio() * cameraZoom;
	cameraPos.y = cameraZoom;
}

void Renderer::StartUIScene()
{
	
	glDisable(GL_DEPTH_TEST);

	uiRd->cameraController->SetZoomLevel(cameraZoom);
	uiRd->cameraController->SetPosition(cameraPos);

	uiRd->vao->Bind();
	uiRd->shader->Use();
	uiRd->shader->UniformMat4("u_ViewProjectionMatrix", uiRd->cameraController->GetCamera().GetViewProjection());
	uiRd->shader->UniformInt("uTextureSampler", 1);

	UIResetBatch();
}

void Renderer::EndUIScene()
{
	UIFlushBatch();
}

void Renderer::UIDrawTexture(const Texture* texture, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour, float flag)
{
	if (uiRd->drawCount >= UIRenderData::MAX_DRAWS)
		UIFlushAndReset();

	glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3{ pos.x, pos.y, UIRenderData::VERTEX_DEPTH_VALUE }), { size, 1.0f });

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
		SetUIVertexData(uiRd->batchPtr, transform * uiRd->BatchQuadVertices[i], texCoordsArray[i], colour, flag);
		++uiRd->batchPtr;
	}

	uiRd->drawCount++;
}

void Renderer::UIDrawRectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour)
{
	UIDrawTexture(Font::DEFAULT->GetBlockSubTexture(), pos, size, colour, 1);
}

float inline GetAbsoluteUIFloat(const UIFloat uiFloat, const float contextualSize)
{
	switch (uiFloat.type)
	{
	case UIData::PERCENTAGE:
		return uiFloat.val * contextualSize;
	case UIData::PIXEL:
		return uiFloat.val;
	case UIData::UNIT:
	default:
		return 0;
	}
	return 0;
}

glm::vec2 inline GetAbsoluteUIVector(const UIVector& uiVector, const glm::vec2& contextualSize)
{

}

void Renderer::UIDrawRectangle(const UIVector& pos, const UIVector& size, const glm::vec4& colour)
{
}


void Renderer::UIDrawChar(Font* font, const char c, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour)
{
	const SubTexture* charSubTexture = font->GetCharSubTexture(c);
	UIDrawTexture(charSubTexture, pos, size, colour, 1);
}

void Renderer::UIDrawText(Font* font, const std::string& text, const glm::vec2& pos, const glm::vec2& charSize, const glm::vec4& colour, float charSpacingPercent)
{
	float xOffset = pos.x;
	for (char c : text)
	{
		if (c != ' ')
		{
			glm::vec2 charPos{ xOffset, pos.y };
			Renderer::UIDrawChar(font, c, charPos, charSize, colour);
		}
		xOffset += charSpacingPercent * charSize.x;
	}

}


void Renderer::UIFlushBatch()
{
	if (uiRd->drawCount == 0)
		return;

	uint32_t dataSize = static_cast<uint32_t>((uint8_t*)uiRd->batchPtr - (uint8_t*)uiRd->batchBasePtr);

	uiRd->shader->Use();
	uiRd->vao->Bind();
	uiRd->vao->GetVertexBuffer()->SetData(uiRd->batchBasePtr, dataSize);

	uiRd->shader->UniformFloat("uTime", Platform::GetElapsedTime());

	Font::DEFAULT->GetTexture()->Bind(1);
	
	glDrawElements(GL_TRIANGLES, uiRd->drawCount * 6, GL_UNSIGNED_INT, 0);
}

void Renderer::UIResetBatch()
{
	uiRd->batchPtr = uiRd->batchBasePtr;
	uiRd->drawCount = 0;
}

void Renderer::UIFlushAndReset()
{
	UIFlushBatch();
	UIFlushAndReset();
}

inline void Renderer::SetUIVertexData(UIVertex* ptr, const glm::vec4& pos, const glm::vec2& texCoords, const glm::vec4& colour, const float flags)
{
	ptr->pos = pos;
	ptr->texCoords = texCoords;
	ptr->colour = colour;
	ptr->flags = flags;
}


void Renderer::ImGuiDebug()
{
	ImGui::DragFloat2("Camera pos", (float*)&cameraPos);
	ImGui::DragFloat("Camera zoom", &cameraZoom);
}
