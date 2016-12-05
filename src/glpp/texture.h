#ifndef GLPP_TEXTURE_H
#define GLPP_TEXTURE_H

#include <string>
#include <vector>
#include <memory>

#include <GL/glew.h>

namespace glpp
{

class Texture;
using TexturePtr = std::shared_ptr<Texture>;

class Texture
{
public:
	explicit Texture(GLuint texture);
	~Texture();

	Texture(Texture const &) = delete;
	Texture & operator=(Texture const &) = delete;

	static TexturePtr load1D(std::string const & path);
	static TexturePtr load2D(std::string const & path);
	static TexturePtr loadCubemap(std::vector<std::string> const & faces);

	operator GLuint() const;

private:
	GLuint m_handle;
};

}	// namespace glpp

#endif // GLPP_TEXTURE_H
