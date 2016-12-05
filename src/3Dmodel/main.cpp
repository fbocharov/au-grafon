#include "model_render.h"

#include <iostream>
#include <vector>

#include <glpp/program.h>
#include <glpp/shader.h>

int main()
{
	std::vector<std::string> skyboxFaces = {
		"textures/right.png",
		"textures/left.png",
		"textures/top.png",
		"textures/bottom.png",
		"textures/back.png",
		"textures/front.png"
	};

	auto render = std::make_shared<ModelRender>(1280, 720, "models/bunny.obj", skyboxFaces);

	auto skyboxProg = std::make_shared<glpp::Program>(std::vector<glpp::ShaderPtr>{
		std::make_shared<glpp::Shader>(GL_VERTEX_SHADER, "shaders/skybox.vert"),
		std::make_shared<glpp::Shader>(GL_FRAGMENT_SHADER, "shaders/skybox.frag")
	});
	render->setSkyboxProgram(skyboxProg);

	auto modelProg = std::make_shared<glpp::Program>(std::vector<glpp::ShaderPtr>{
		std::make_shared<glpp::Shader>(GL_VERTEX_SHADER, "shaders/bunny.vert"),
		std::make_shared<glpp::Shader>(GL_FRAGMENT_SHADER, "shaders/bunny.frag")
	});
	render->setModelProgram(modelProg);

	while (!render->isStopped())
		render->render();

	return 0;
}
