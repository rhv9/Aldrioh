#pragma once
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"

struct BatchVertex
{
	glm::vec4 pos{ 0 };
	glm::vec2 texCoord{ 0 };
};

class Renderer
{
public:

	static void Init();

	static void StartScene(const Camera& camera);


	static void DrawQuad(const glm::vec3& position, const glm::vec2& scale = { 1.0f, 1.0f });

	static void DrawQuad(const glm::vec3& position, const std::shared_ptr<Texture>& texture, const glm::vec2& scale = { 1.0f, 1.0f });
	static void DrawQuad(const glm::vec3& position, const Texture* texture, const glm::vec2& scale = { 1.0f, 1.0f });

	static void SetRenderDepthOnly(bool val);
	static bool IsRenderDepth();

	static void EndScene();
	static void Destroy();

	static void FlushBatch();
	static void ResetBatch();

private:
	static inline void SetBatchVertexBuffer(BatchVertex* ptr, const glm::vec4& pos, const glm::vec2& texCoords);
};