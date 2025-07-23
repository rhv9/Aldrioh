#pragma once
#include "Shader.h"

enum class ShaderUniformType : uint32_t
{
	Int = 0,
	UInt,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
};

struct ShaderUniform
{
	ShaderUniformType
};

struct ShaderConfig
{
	Shader& shader;
	std::vector<
};