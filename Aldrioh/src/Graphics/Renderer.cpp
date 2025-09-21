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

	EventCallbackID<WindowResizeEventArg> callbackWindowResizeID;
};

static RenderData renderData;

void Renderer::Init()
{
	LOG_CORE_INFO("Renderer::Init()");

	renderData.callbackWindowResizeID = Game::Instance().GetWindow()->WindowResizeEventHandler += Renderer::OnResize;

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

void Renderer::DrawQuad(const glm::vec3& position, const std::shared_ptr<SubTexture>& subTexture, const glm::vec2& scale)
{
	DrawQuad(position, subTexture.get(), scale);
}

void inline Renderer::SetBatchVertexBuffer(BatchVertex* ptr, const glm::vec4& pos, const glm::vec2& texCoords)
{
	ptr->pos = pos;
	ptr->texCoord = texCoords;
}

void Renderer::DrawQuad(const glm::vec3& position, const SubTexture* subTexture, const glm::vec2& scale)
{
	if (renderData.drawCount >= RenderData::MAX_DRAWS)
		FlushAndReset();

	glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), position), { scale, 1.0f });

	const TextureCoords& texCoords = subTexture->textureCoords;
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

	Sprites::get(Sprites::fire)->textureParent->Bind(0);

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
	float textureSampler;
	float flags;
};

struct UIRenderData
{
	Shader* shader = nullptr;
	std::unique_ptr<VertexArray> vao;

	CameraController* cameraController;
	float cameraZoom = 1080 / 2;
	glm::vec2 cameraPos{ 0 };
	glm::vec2 WindowSize{ 0 };

	// Batching Textures
	static const uint32_t MAX_DRAWS = 1000;
	static const uint32_t MAX_BATCH_VERTICES = MAX_DRAWS * 4;
	static const uint32_t MAX_BATCH_INDICES = MAX_DRAWS * 6;

	static const uint32_t MAX_TEXTURE = 8;

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

	int textureSlots[MAX_TEXTURE];
	uint32_t ptr = 0;
};

static UIRenderData* uiRd;

void Renderer::InitUIRenderer()
{
	LOG_CORE_INFO("Initializing UI Renderer");

	uiRd = new UIRenderData();
	float aspectRatio = Game::Instance().GetWindow()->GetAspectRatio();
	uiRd->cameraController = new CameraController(aspectRatio, 1);
	uiRd->cameraController->SetZoomLevel(uiRd->cameraZoom);
	uiRd->cameraController->SetPosition(uiRd->cameraPos);

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
	{ "aTextureSampler", VertexAttrib::Float, false},
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

	// Texture slots
	int textureBindings[uiRd->MAX_TEXTURE];
	for (int i = 0; i < uiRd->MAX_TEXTURE; ++i)
	{
		uiRd->textureSlots[i] = 0;
		textureBindings[i] = i;
	}
	uiRd->shader->Use();
	uiRd->shader->UniformIntArray("uTextureSamplers", &textureBindings[0], uiRd->MAX_TEXTURE);

	UIResize(Game::Instance().GetWindow()->GetWidth(), Game::Instance().GetWindow()->GetHeight());

	delete[] batchIndices;
}

void Renderer::DestroyUIRenderer()
{
	LOG_CORE_INFO("Destroying UI Renderer");

	delete uiRd->cameraController;
	delete uiRd;
}

void Renderer::UIOnResize(WindowResizeEventArg& e)
{
	UIResize(e.Width, e.Height);
}

void inline Renderer::UIResize(uint32_t width, uint32_t height)
{
	uiRd->cameraController->OnResize(width, height);
	uiRd->cameraPos.x = uiRd->cameraController->GetAspectRatio() * uiRd->cameraZoom;
	uiRd->cameraPos.y = uiRd->cameraZoom;
	uiRd->WindowSize = { width, height };
}

glm::vec2 Renderer::UIGetWindowSize()
{
	return uiRd->cameraController->GetBounds().GetSize();
}

void Renderer::SetUIPixelHeight(float height)
{
	uiRd->cameraZoom = height / 2;
	uiRd->cameraController->SetZoomLevel(uiRd->cameraZoom);
	UIResize(uiRd->WindowSize.x, uiRd->WindowSize.y);
}

void Renderer::StartUIScene()
{
	glDisable(GL_DEPTH_TEST);

	uiRd->cameraController->SetZoomLevel(uiRd->cameraZoom);
	uiRd->cameraController->SetPosition(uiRd->cameraPos);

	uiRd->vao->Bind();
	uiRd->shader->Use();
	uiRd->shader->UniformMat4("u_ViewProjectionMatrix", uiRd->cameraController->GetCamera().GetViewProjection());

	uiRd->ptr = 0;

	UIResetBatch();
}

void Renderer::EndUIScene()
{
	UIFlushBatch();
}

void Renderer::UIDrawTexture(const SubTexture* subTexture, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour, float flag)
{
	if (uiRd->drawCount >= UIRenderData::MAX_DRAWS)
		UIFlushAndReset();

	// Texture slot
	uint32_t textureSlot = -1;
	for (int i = 0; i < uiRd->ptr; ++i)
	{
		if (uiRd->textureSlots[i] == subTexture->textureParent->GetTextureId())
		{
			textureSlot = i;
			break;
		}
	}
	
	if (textureSlot == -1)
	{
		textureSlot = uiRd->ptr;
		uiRd->textureSlots[uiRd->ptr++] = subTexture->textureParent->GetTextureId();
	}

	glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3{ pos.x, pos.y, UIRenderData::VERTEX_DEPTH_VALUE }), { size, 1.0f });

	const TextureCoords& texCoords = subTexture->textureCoords;
	const glm::vec2 texCoordsArray[4] =
	{
		{ texCoords.bottomLeft.x, texCoords.topRight.y   },
		{ texCoords.bottomLeft.x, texCoords.bottomLeft.y },
		{ texCoords.topRight.x,   texCoords.bottomLeft.y },
		{ texCoords.topRight.x,   texCoords.topRight.y   },
	};

	for (int i = 0; i < 4; ++i)
	{
		SetUIVertexData(uiRd->batchPtr, transform * uiRd->BatchQuadVertices[i], texCoordsArray[i], colour, textureSlot, flag);
		++uiRd->batchPtr;
	}

	uiRd->drawCount++;
}

bool flipY = false;
int anchorPoint = 0;
AnchorPoint uiAnchorPoint = AnchorPoint::LEFT_BOTTOM;

void Renderer::UIDrawRectangle(const UIVector& pos, const UIVector& size, const glm::vec4& colour)
{
	glm::vec2 absolutePos = pos.GetAbsolute(uiRd->WindowSize);
	glm::vec2 absoluteSize = size.GetAbsolute(uiRd->WindowSize);

	UIDrawTexture(Font::DEFAULT->GetBlockSubTexture(), absolutePos, absoluteSize, colour, 1);
}


void Renderer::UIDrawRectangle(const UIVector& pos, const UIVector& size, const glm::vec4& colour, AnchorPoint ap)
{
	glm::vec2 absolutePos = pos.GetAbsolute(uiRd->WindowSize);
	glm::vec2 absoluteSize = size.GetAbsolute(uiRd->WindowSize);

	absolutePos = ap.ConvertPos(absolutePos, absoluteSize, uiRd->cameraController->GetBounds().GetSize());

	UIDrawTexture(Font::DEFAULT->GetBlockSubTexture(), absolutePos, absoluteSize, colour, 1);
}

void Renderer::UIDrawChar(Font* font, const char c, const UIVector& pos, const UIVector& size, const glm::vec4& colour, AnchorPoint ap)
{
	const SubTexture* charSubTexture = font->GetCharSubTexture(c);
	glm::vec2 absolutePos = pos.GetAbsolute(uiRd->WindowSize);
	glm::vec2 absoluteSize = size.GetAbsolute(uiRd->WindowSize);

	absolutePos = ap.ConvertPos(absolutePos, absoluteSize, uiRd->cameraController->GetBounds().GetSize());

	UIDrawTexture(charSubTexture, absolutePos, absoluteSize, colour, 1);
}

float GetTextWidth(const std::string& text, float fontSize, float charSpacingPercent)
{
	return text.size() * fontSize * charSpacingPercent;
}

void Renderer::UIDrawText(Font* font, const std::string& text, const UIVector& pos, float fontSize, const glm::vec4& colour, float charSpacingPercent, AnchorPoint ap)
{
	glm::vec2 absolutePos = pos.GetAbsolute(uiRd->WindowSize);
	glm::vec2 absoluteSize = glm::vec2(fontSize, fontSize);

	float textWidth = GetTextWidth(text, fontSize, charSpacingPercent);

	absolutePos = ap.ConvertPos(absolutePos, { textWidth, fontSize }, uiRd->cameraController->GetBounds().GetSize());

	for (char c : text)
	{
		if (c != ' ')
		{
			const SubTexture* charSubTexture = font->GetCharSubTexture(c);
			UIDrawTexture(charSubTexture, absolutePos, absoluteSize, colour, 1);
		}
		absolutePos.x += charSpacingPercent * fontSize;
	}

}

void Renderer::UIDrawImage(const SubTexture* subTexture, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour)
{
	UIDrawTexture(subTexture, pos, size, colour, 0);
}

void Renderer::UIFlushBatch()
{
	if (uiRd->drawCount == 0)
		return;

	uint32_t dataSize = static_cast<uint32_t>((uint8_t*)uiRd->batchPtr - (uint8_t*)uiRd->batchBasePtr);

	uiRd->shader->Use();
	uiRd->shader->UniformFloat("uTime", Platform::GetElapsedTime());
	
	uiRd->vao->Bind();
	uiRd->vao->GetVertexBuffer()->SetData(uiRd->batchBasePtr, dataSize);

	for (int slot = 0; slot < uiRd->ptr; ++slot)
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTextureUnit(slot, uiRd->textureSlots[slot]);
	}

	glDrawElements(GL_TRIANGLES, uiRd->drawCount * 6, GL_UNSIGNED_INT, 0);
}

void Renderer::UIResetBatch()
{
	uiRd->batchPtr = uiRd->batchBasePtr;
	uiRd->drawCount = 0;
	uiRd->ptr = 0;
}

void Renderer::UIFlushAndReset()
{
	UIFlushBatch();
	UIResetBatch();
}

inline void Renderer::SetUIVertexData(UIVertex* ptr, const glm::vec4& pos, const glm::vec2& texCoords, const glm::vec4& colour, const uint32_t slot, const float flags)
{
	ptr->pos = pos;
	ptr->texCoords = texCoords;
	ptr->colour = colour;
	ptr->textureSampler = slot;
	ptr->flags = flags;
}


void Renderer::ImGuiDebug()
{
	if (ImGui::DragFloat2("Camera relativePos", (float*)&uiRd->cameraPos))
	{
		UIResize(uiRd->WindowSize.x, uiRd->WindowSize.y);
	}
	if (ImGui::DragFloat("Camera zoom", &uiRd->cameraZoom))
	{
		UIResize(uiRd->WindowSize.x, uiRd->WindowSize.y);
	}
	ImGui::DragFloat2("UIRd->WindowSize", (float*)&uiRd->WindowSize);
	glm::vec2 cameraBounds = uiRd->cameraController->GetBounds().GetSize();
	ImGui::DragFloat2("CameraBounds", (float*)&cameraBounds);
	ImGui::Checkbox("Flip UI on Y", &flipY);
	if(ImGui::SliderInt("AnchorPoint ", &anchorPoint, 0, 8))
	{
		uiAnchorPoint = (AnchorPoint::Value)anchorPoint;
	}
}
