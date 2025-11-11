#include <pch.h>
#include "ShaderManager.h"

ShaderManager ShaderManager::shaderManager;

void ShaderManager::LoadShaders()
{
	LOG_CORE_INFO("ShaderManager::LoadShaders()");

	shaders[static_cast<uint32_t>(ShaderName::GENERAL_TEXTURE)] = { "assets/shaders/TextureTexCoord.glsl" };
	shaders[static_cast<uint32_t>(ShaderName::BATCH_TEXTURE)] = { "assets/shaders/BatchTexture.glsl" };
	shaders[static_cast<uint32_t>(ShaderName::UI_SHADER)] = { "assets/shaders/UIShader.glsl" };
	
	shaders[static_cast<uint32_t>(ShaderName::BACKGROUND_SHADER)] = { "assets/shaders/Background.glsl" };

}
