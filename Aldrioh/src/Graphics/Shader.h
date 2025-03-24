#pragma once
#include <glad/glad.h>

class Shader
{
public:
	Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath);
	Shader(const std::string& path);

	~Shader();

	void UniformInt(const std::string& name, const int val) const;
	void UniformUInt(const std::string& name, const uint32_t val) const;
	void UniformIntArray(const std::string& name, int* values, uint32_t count) const;
	void UniformFloatArray(const std::string& name, float* values, uint32_t count) const;

	void UniformFloat(const std::string& name, const float val) const;
	void UniformFloat2(const std::string& name, const glm::vec2& vector) const;
	void UniformFloat3(const std::string& name, const glm::vec3& vector) const;
	void UniformFloat4(const std::string& name, const glm::vec4& vector) const;

	void UniformMat3(const std::string& name, const glm::mat3& matrix) const;
	void UniformMat4(const std::string& name, const glm::mat4& matrix) const;

	void Use();
private:
	GLuint m_Program;
};

