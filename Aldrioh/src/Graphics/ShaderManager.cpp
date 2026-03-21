#include <pch.h>
#include "ShaderManager.h"
#include <Core/Platform.h>

#include <Game.h>

ShaderManager ShaderManager::shaderManager;

void ShaderManager::LoadShaders()
{
	LOG_CORE_INFO("ShaderManager::LoadShaders()");

	shaders[static_cast<uint32_t>(ShaderName::GENERAL_TEXTURE)] = { "assets/shaders/TextureTexCoord.glsl" };
	shaders[static_cast<uint32_t>(ShaderName::BATCH_TEXTURE)] = { "assets/shaders/BatchTexture.glsl" };
	shaders[static_cast<uint32_t>(ShaderName::UI_SHADER)] = { "assets/shaders/UIShader.glsl" };

	shaders[static_cast<uint32_t>(ShaderName::BACKGROUND_SHADER)] = { "assets/shaders/Background.glsl" };
}

void ShaderManager::EnableRecompileShaderOnEdit(const ShaderName shaderName)
{
	Platform::File::WatchForFileUpdate(GetShader(shaderName).GetFilePath(), [shaderName](const std::string& filePath)
		{
			Game::Instance().AddMainThreadJob([shaderName]() { ShaderManager::Get().GetShader(shaderName).Recompile(); });
		});
}
