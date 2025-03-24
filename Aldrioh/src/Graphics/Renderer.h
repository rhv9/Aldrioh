#pragma once
#include "Camera.h"
#include "Texture.h"
#include "Shader.h"

class Renderer
{
public:

	static void Init();

	static void StartScene(const Camera& camera);

	static void DrawQuad(const glm::vec3& position, const glm::vec2& scale = { 1.0f, 1.0f });

	static void DrawQuad(const glm::vec3& position, const std::shared_ptr<Texture>& texture, const glm::vec2& scale = { 1.0f, 1.0f });
	static void DrawQuad(const glm::vec3& position, const Texture* texture, const glm::vec2& scale = { 1.0f, 1.0f });

	static void DrawQuadCustomShader(const Ref<Shader>& shader, const glm::vec3& position, const Texture* texture, const glm::vec2& scale = { 1.0f, 1.0f });

	static void DrawQuadFlatColour(const glm::vec3& position, const glm::vec4& colour,const glm::vec2& scale = { 1.0f, 1.0f });

	static void SetRenderDepthOnly(bool val);
	static bool IsRenderDepth();

	static void EndScene();
};