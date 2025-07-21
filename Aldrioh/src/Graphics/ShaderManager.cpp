#include <pch.h>
#include "ShaderManager.h"

ShaderManager ShaderManager::shaderManager;

void ShaderManager::LoadShaders()
{
	LOG_CORE_INFO("ShaderManager::LoadShaders()");

	shaders[static_cast<uint32_t>(ShaderName::GENERAL_TEXTURE)] = { "assets/shaders/TextureTexCoord.glsl" };
	shaders[static_cast<uint32_t>(ShaderName::OTHER_TEXTURE)] = { "assets/shaders/Texture.glsl" };
}
