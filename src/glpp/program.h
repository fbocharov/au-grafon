#ifndef GLPP_PROGRAM_H
#define GLPP_PROGRAM_H

#include "shader.h"

#include <vector>
#include <memory>

#include <GL/glew.h>

namespace glpp
{

class Program
{
public:
	Program(std::vector<ShaderPtr> const & shaders);
	~Program();

	Program(Program const & other) = delete;
	Program & operator=(Program const & other) = delete;

	operator GLuint() const;

	void setUniform(std::string const & name, std::vector<float> const & values);

private:
	GLuint m_handle;
};
using ProgramPtr = std::shared_ptr<Program>;

}	// namespace glpp

#endif  // GLPP_PROGRAM_H
