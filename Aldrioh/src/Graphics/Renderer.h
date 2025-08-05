#pragma once
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"
#include <UI/Font.h>

struct BatchVertex;

class Renderer
{
public:

	static void Init();

	static void StartScene(const Camera& camera);

	static void DrawQuad(const glm::vec3& position, const glm::vec2& scale = { 1.0f, 1.0f });

	static void DrawQuad(const glm::vec3& position, const std::shared_ptr<Texture>& texture, const glm::vec2& scale = { 1.0f, 1.0f });
	static void DrawQuad(const glm::vec3& position, const Texture* texture, const glm::vec2& scale = { 1.0f, 1.0f });

	static void EndScene();
	static void Destroy();

	static void FlushBatch();
	static void ResetBatch();

	static void FlushAndReset();

	// UI related

	static void StartUIScene();
	static void EndUIScene();

	static void UIDrawRectangle();
	static void UIDrawText(const std::string& text);
	static void UIDrawChar(Font* font, const char c, const glm::vec2& pos, const glm::vec2& size);

private:
	static void InitUIRenderer();
	static void DestroyUIRenderer();

private:
	static inline void SetBatchVertexBuffer(BatchVertex* ptr, const glm::vec4& pos, const glm::vec2& texCoords);
};