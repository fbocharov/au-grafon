#ifndef GLPP_MESH_H
#define GLPP_MESH_H

#include "program.h"
#include "texture.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace glpp
{

class Mesh
{
public:
	struct Vertex
	{
		glm::vec3 m_position;
		glm::vec3 m_normal;
		glm::vec2 m_textureCoords;
	};

public:
	Mesh(std::vector<Vertex> const & vertices,
		 std::vector<GLuint> const & indices,
		 std::vector<TexturePtr> const & textures);
	~Mesh();

	Mesh(Mesh const &) = delete;
	Mesh & operator=(Mesh const &) = delete;

	void render(ProgramPtr program);

private:
	GLuint m_vao;
	GLuint m_vbo;
	GLuint m_ebo;

	std::vector<TexturePtr> m_textures;
	size_t m_trianglesCount;
};
using MeshPtr = std::shared_ptr<Mesh>;

}	// namespace glpp

#endif // GLPP_MESH_H
