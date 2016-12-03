#ifndef GLPP_SHADER_H
#define GLPP_SHADER_H

#include <string>
#include <memory>

#include <GL/glew.h>

namespace glpp
{

class Shader
{
public:
	Shader(GLenum type, std::string const & shaderFilePath);
	~Shader();

	Shader(Shader const & other) = delete;
	Shader & operator=(Shader const & other) = delete;

	operator GLuint() const;

private:
	GLuint m_handle;
};
using ShaderPtr = std::shared_ptr<Shader>;

}	// namespace glpp

#endif	// GLPP_SHADER_H
