#include "fractal_render.h"

#include <glpp/render.h>
#include <glpp/program.h>
#include <glpp/shader.h>

#include <iostream>
#include <cmath>
#include <memory>

int main()
{
	auto render = std::make_unique<FractalRender>(800, 600, "textures/mandelbrot.png");

	auto program = std::make_shared<glpp::Program>(std::vector<glpp::ShaderPtr>{
		{ std::make_shared<glpp::Shader>(GL_VERTEX_SHADER, "shaders/vertex.glsl") },
		{ std::make_shared<glpp::Shader>(GL_FRAGMENT_SHADER, "shaders/fragment.glsl") }
	});

	render->setProgram(program);
	while (!render->isStopped()) {
		render->render();
	}

	return 0;
}
