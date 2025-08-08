#pragma once
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include <UI/Font.h>

#include <Core/Window.h>

struct BatchVertex;
struct UIVertex;

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
	static void DrawQuad(const glm::vec3& position, const std::shared_ptr<Texture>& texture, const glm::vec2& scale = { 1.0f, 1.0f });
	static void DrawQuad(const glm::vec3& position, const Texture* texture, const glm::vec2& scale = { 1.0f, 1.0f });

	static void FlushBatch();
	static void ResetBatch();
	static void FlushAndReset();

private:
	static inline void SetBatchVertexBuffer(BatchVertex* ptr, const glm::vec4& pos, const glm::vec2& texCoords);

public:
	// UI related
	static void StartUIScene();
	static void EndUIScene();

	static void UIDrawTexture(const Texture* texture, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour, float flag);

	static void UIDrawRectangle(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour);
	static void UIDrawChar(Font* font, const char c, const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour);
	static void UIDrawText(Font* font, const std::string& text, const glm::vec2& pos, const glm::vec2& charSize, const glm::vec4& colour, float charSpacingPercent = 0.85f);

	static void UIFlushBatch();
	static void UIResetBatch();
	static void UIFlushAndReset();

private:
	static void InitUIRenderer();
	static void DestroyUIRenderer();

	static void UIOnResize(WindowResizeEventArg& e);

	static inline void SetUIVertexData(UIVertex* ptr, const glm::vec4& pos, const glm::vec2& texCoords, const glm::vec4& colour, const float flags);

};