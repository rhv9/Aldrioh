#include "pch.h"
#include "Shader.h"

#include "Core/Platform.h"

static GLenum ShaderTypeFromString(std::string& type)
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    else if (type == "fragment")
        return GL_FRAGMENT_SHADER;

    return 0;
}

std::unordered_map<GLenum, std::string> PreProcess(const std::string& source)
{
    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);

    ASSERT(pos != std::string::npos, "No declaration of type of shader found!");

    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);

        ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");
        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        ASSERT(nextLinePos != std::string::npos, "Syntax Error!");
        pos = source.find(typeToken, nextLinePos);
        shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
    }

    return shaderSources;
}




GLuint CreateShader(GLenum shaderType, const char* shaderSource)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string stringShaderType;
        switch (shaderType)
        {
        case GL_VERTEX_SHADER:
            stringShaderType = "Vertex";
            break;
        case GL_FRAGMENT_SHADER:
            stringShaderType = "Fragment";
            break;
        default:
            LOG_CORE_WARN("Shader Warning: Creating shaderType that is not expected.");
        }

        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG_CORE_ERROR("Error: {} Shader failed to compile. Info Log: \n{}", stringShaderType, infoLog);
    }

    return shader;
}

Shader::Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath)
{
    std::string vertexSource = Platform::File::ReadFileAsString(vertexSourcePath.c_str());
    std::string fragmentSource = Platform::File::ReadFileAsString(fragmentSourcePath.c_str());

    GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexSource.c_str());
    GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());

    int success;
    char infoLog[512];
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        LOG_CORE_ERROR("Error: Program failed to link with shaders: {} {}, Info Log: \n{}", vertexSourcePath, fragmentSourcePath, infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_Program = shaderProgram;

    LOG_CORE_INFO("Shader created successfully: {}, {}", vertexSourcePath, fragmentSourcePath);
}

Shader::Shader(const std::string& path)
{
    std::string shaderSource = Platform::File::ReadFileAsString(path.c_str());

    ASSERT(shaderSource != "", "File location gave empty string.");

    std::unordered_map<GLenum, std::string> shaderSources = PreProcess(shaderSource);

    GLuint shaderIDs[2];
    int shaderPos = 0;

    for (auto&& [type, source] : shaderSources)
    {
        shaderIDs[shaderPos++] = CreateShader(type, source.c_str());
    }

    ASSERT(shaderPos == 2, "Unsupported number of shaders added");

    int success;
    char infoLog[512];
    // link shaders
    unsigned int shaderProgram = glCreateProgram();

    for (GLuint id : shaderIDs)
        glAttachShader(shaderProgram, id);

    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        LOG_CORE_ERROR("Error: Program failed to link with shaders: {}, Info Log: \n{}", path, infoLog);
    }

    for (GLuint id : shaderIDs)
        glDeleteShader(id);

    m_Program = shaderProgram;

    LOG_CORE_INFO("Shader created successfully: {}", path);
}


void Shader::Use()
{
	glUseProgram(m_Program);
}

Shader::~Shader()
{
    glDeleteProgram(m_Program);
}

void Shader::UniformInt(const std::string& name, const int val) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniform1i(location, val);
}

void Shader::UniformUInt(const std::string& name, const uint32_t val) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniform1ui(location, val);
}

void Shader::UniformIntArray(const std::string& name, int* values, uint32_t count) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniform1iv(location, count, values);
}

void Shader::UniformFloatArray(const std::string& name, float* values, uint32_t count) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniform1fv(location, count, values);
}

void Shader::UniformFloat(const std::string& name, const float val) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniform1f(location, val);
}

void Shader::UniformFloat2(const std::string& name, const glm::vec2& vector) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}

void Shader::UniformFloat3(const std::string& name, const glm::vec3& vector) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::UniformFloat4(const std::string& name, const glm::vec4& vector) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void Shader::UniformMat3(const std::string& name, const glm::mat3& matrix) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UniformMat4(const std::string& name, const glm::mat4& matrix) const
{
    int location = glGetUniformLocation(m_Program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
