#pragma once
#include "Shader.h"

enum class ShaderName : uint32_t
{
	GENERAL_TEXTURE = 0,
	BATCH_TEXTURE,
	UI_SHADER,
};

class ShaderManager
{
public:
	static ShaderManager& Get() { return shaderManager; }

	void LoadShaders();
	Shader& GetShader(const ShaderName shaderName) { return shaders[static_cast<uint32_t>(shaderName)]; }

private:
	ShaderManager() {}

private:
	static ShaderManager shaderManager;

	// This is not good, have to manually remember to increase this;
	std::array<Shader, 5> shaders;
};