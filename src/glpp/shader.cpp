#include "shader.h"

#include <fstream>
#include <stdexcept>
#include <vector>

namespace glpp
{

namespace
{

std::string readFileAsString(std::string const & path)
{
	std::ifstream file(path);
	return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}

}	// namespace

Shader::Shader(GLenum type, std::string const & shaderFilePath)
	: m_handle(glCreateShader(type))
{
	auto code = readFileAsString(shaderFilePath);
	char const * codeStr = code.c_str();
	glShaderSource(m_handle, 1, &codeStr, nullptr);
	glCompileShader(m_handle);

	GLint success;
	glGetShaderiv(m_handle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLint logLength;
		glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength + 1); // +1 for '\0'

		glGetShaderInfoLog(m_handle, 512, NULL, &log[0]);
		throw std::runtime_error("failed to compile " + shaderFilePath + ": " + &log[0]);
	}
}

Shader::operator GLuint() const
{
	return m_handle;
}

Shader::~Shader()
{
	glDeleteShader(m_handle);
}

}	// namespace glpp
