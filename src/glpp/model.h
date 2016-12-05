#ifndef GLPP_MODEL_H
#define GLPP_MODEL_H

#include "mesh.h"
#include "program.h"
#include "texture.h"

#include <string>
#include <vector>

#include <assimp/scene.h>

namespace glpp
{

class Model
{
public:
	Model(std::string const & path);

	void render(ProgramPtr program);

private:
	void process(aiScene const * scene, aiNode * node, std::string const & modelDir);
	MeshPtr createMesh(aiMesh * mesh, aiScene const * scene, std::string const & modelDir);

private:
	std::vector<MeshPtr> m_meshes;
};

}	// namespace glpp

#endif // GLPP_MODEL_H
