#ifndef GLPP_TEXTURE_H
#define GLPP_TEXTURE_H

#include <string>

#include <GL/glew.h>

namespace glpp
{

class Texture
{
public:
	explicit Texture(GLuint texture);

	static Texture load1D(std::string const & path);

	operator GLuint() const;

private:
	GLuint m_handle;
};

}	// namespace glpp

#endif // GLPP_TEXTURE_H
