#include "program.h"

#include <stdexcept>
#include <vector>

namespace glpp
{

Program::Program(std::vector<ShaderPtr> const & shaders)
	: m_handle(glCreateProgram())
{
	for (auto shader: shaders)
		glAttachShader(m_handle, *shader);

	glLinkProgram(m_handle);

	GLint success;
	glGetShaderiv(m_handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint logLength;
		glGetShaderiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength + 1); // +1 for '\0'

		glGetShaderInfoLog(m_handle, 512, NULL, &log[0]);
		throw std::runtime_error(std::string("failed to link program: ") + &log[0]);
	}

	for (auto shader: shaders)
		glDetachShader(m_handle, *shader);
}

Program::~Program()
{
	glDeleteProgram(m_handle);
}

Program::operator GLuint() const
{
	return m_handle;
}

void Program::setUniform(std::string const & name, std::vector<float> const & values)
{
	GLint location = glGetUniformLocation(m_handle, name.c_str());
	switch (values.size()) {
		case 1:
			glUniform1f(location, values[0]);
			break;
		case 2:
			glUniform2f(location, values[0], values[1]);
			break;
		case 3:
			glUniform3f(location, values[0], values[1], values[2]);
			break;
		case 4:
			glUniform4f(location, values[0], values[1], values[3], values[3]);
			break;
		default:
			throw std::runtime_error("wrong number of arguments for setUniform: "
									 + std::to_string(values.size()));
	}
}


}	// namespace glpp
