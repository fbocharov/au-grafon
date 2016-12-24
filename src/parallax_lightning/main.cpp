#include <iostream>
#include <memory>

#include "bricks_render.h"

int main()
{
	auto render = std::make_shared<BricksRender>(800, 600,
		"textures/bricks.jpg", "textures/bricks_norm.jpg", "textures/bricks_disp.jpg");
	auto program = std::make_shared<glpp::Program>(std::vector<glpp::ShaderPtr>{
		{ std::make_shared<glpp::Shader>(GL_VERTEX_SHADER, "shaders/bricks.vert") },
		{ std::make_shared<glpp::Shader>(GL_FRAGMENT_SHADER, "shaders/bricks.frag") }
	});

	render->setProgram(program);
	while (!render->isStopped()) {
		render->render();
	}

	return 0;
}
