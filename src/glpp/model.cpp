#include "model.h"
#include "mesh.h"
#include "program.h"

#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/SOIL.h>

namespace glpp
{

namespace
{

std::map<aiTextureType, std::string> const TYPE_NAMES = {
	{ aiTextureType_DIFFUSE, "texture_diffuse" },
	{ aiTextureType_SPECULAR, "texture_specular" },
	{ aiTextureType_AMBIENT, "texture_reflection" }
};

}	// namespace

Model::Model(std::string const & path)
{
	Assimp::Importer importer;

	aiScene const * scene = importer.ReadFile(path.c_str(), aiProcessPreset_TargetRealtime_Quality);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		throw std::runtime_error("failed to load model from " + path);

	auto idx = path.find_last_of("/");
	auto dir = (idx == std::string::npos) ? "." : path.substr(0, idx + 1);
	process(scene, scene->mRootNode, dir);
}

void Model::render(ProgramPtr program)
{
	for (auto & mesh: m_meshes)
		mesh->render(program);
}

void Model::process(aiScene const * scene, aiNode * node, std::string const & modelDir)
{
	for (GLuint i = 0; i < node->mNumMeshes; ++i) {
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(createMesh(mesh, scene, modelDir));
	}

	for (GLuint i = 0; i < node->mNumChildren; ++i)
		process(scene, node->mChildren[i], modelDir);
}

MeshPtr Model::createMesh(aiMesh * mesh, aiScene const * scene, std::string const & modelDir)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<TexturePtr> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; ++i) {
		Mesh::Vertex vertex;

		vertex.m_position.x = mesh->mVertices[i].x;
		vertex.m_position.y = mesh->mVertices[i].y;
		vertex.m_position.z = mesh->mVertices[i].z;

		vertex.m_normal.x = mesh->mNormals[i].x;
		vertex.m_normal.y = mesh->mNormals[i].y;
		vertex.m_normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.m_textureCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.m_textureCoords.y = mesh->mTextureCoords[0][i].y;
		} else
			vertex.m_textureCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}

	return std::make_shared<Mesh>(vertices, indices, textures);
}

}	// namespace glpp
