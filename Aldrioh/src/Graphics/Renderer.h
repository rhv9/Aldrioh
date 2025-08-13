#pragma once
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include <UI/Font.h>

#include <Core/Window.h>

struct BatchVertex;
struct UIVertex;

enum UIData
{
	PERCENTAGE = 0,
	PIXEL,
	UNIT
};
struct UIVector 
{
public:
	UIData type;
	union {
		glm::vec2 val;
		struct {
			float x;
			float y;
		};
	};

	UIVector(UIData type, float x, float y) : type(type), x(x), y(y) {}
	UIVector(UIData type, const glm::vec2& val) : type(type), val(val) {}

	glm::vec2 GetAbsolute(const glm::vec2& contextualSize) const
	{
		switch (type)
		{
		case UIData::PERCENTAGE:
			return val * contextualSize;
		case UIData::PIXEL:
			return val;
		case UIData::UNIT:
		default:
			return glm::vec2{ 0 };
		}
		return glm::vec2{ 0 };
	}
};

struct UIFloat
{
	UIData type;
	float val;

	float GetAbsolute(const float contextualSize) const
	{
		switch (type)
		{
		case UIData::PERCENTAGE:
			return val * contextualSize;
		case UIData::PIXEL:
			return val;
		case UIData::UNIT:
		default:
			return 0;
		}
		return 0;
	}
};

enum AnchorPoint
{
	LEFT_BOTTOM = 0,
	LEFT_CENTER,
	LEFT_TOP,
	CENTER_BOTTOM,
	CENTER,
	CENTER_TOP,
	RIGHT_BOTTOM,
	RIGHT_CENTER,
	RIGHT_TOP,
};


class Renderer
{
public:

	static void Init();
	static void Destroy();

	static void ImGuiDebug();

	static void OnResize(WindowResizeEventArg& e);
	
	static void StartScene(const Camera& camera);
	static void EndScene();

	static void DrawQuad(const glm::vec3& position, const glm::vec2& scale = { 1.0f, 1.0f });
	static void DrawQuad(const glm::vec3& position, const std::shared_ptr<SubTexture>& subTexture, const glm::vec2& scale = { 1.0f, 1.0f });
	static void DrawQuad(const glm::vec3& position, const SubTexture* subTexture, const glm::vec2& scale = { 1.0f, 1.0f });

	static void FlushBatch();
	static void ResetBatch();
	static void FlushAndReset();

private:
	static inline void SetBatchVertexBuffer(BatchVertex* ptr, const glm::vec4& pos, const glm::vec2& texCoords);

public:

	// UI related
	static void StartUIScene();
	static void EndUIScene();

	static void UIDrawTexture(const SubTexture* subTexture, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour, float flag);

	static void UIDrawRectangle(const UIVector& pos, const UIVector& size, const glm::vec4& colour, AnchorPoint ap = AnchorPoint::LEFT_BOTTOM);

	static void UIDrawChar(Font* font, const char c, const UIVector& pos, const UIVector& size, const glm::vec4& colour, AnchorPoint ap = AnchorPoint::LEFT_BOTTOM);
	static void UIDrawText(Font* font, const std::string& text, const UIVector& pos, float fontSize, const glm::vec4& colour, float charSpacingPercent = 0.85f, AnchorPoint ap = AnchorPoint::LEFT_BOTTOM);

	static void UIFlushBatch();
	static void UIResetBatch();
	static void UIFlushAndReset();

private:
	static void InitUIRenderer();
	static void DestroyUIRenderer();

	static void UIOnResize(WindowResizeEventArg& e);
	static void inline UIResize(uint32_t width, uint32_t height);

	static inline void SetUIVertexData(UIVertex* ptr, const glm::vec4& pos, const glm::vec2& texCoords, const glm::vec4& colour, const float flags);

};