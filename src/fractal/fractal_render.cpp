#include "fractal_render.h"

#include <SOIL/SOIL.h>

#include <algorithm>

namespace
{

constexpr GLfloat VERTICES[] =
{
	-1.0f,	-1.0f,	0.0f,
	 1.0f,	-1.0f,	0.0f,
	-1.0f,	 1.0f,	0.0f,
	-1.0f,	 1.0f,	0.0f,
	 1.0f,	 1.0f,	0.0f,
	 1.0f,	-1.0f,	0.0f
};

}	// namespace

FractalRender::FractalRender(int width, int height, std::string const & texturePath)
	: glpp::Render(width, height, "Mandelbrot set")
	, m_texture(glpp::Texture::load1D(texturePath))
	, m_screenDimension(width, height)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), &VERTICES, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);

	glBindVertexArray(0);
}

FractalRender::~FractalRender()
{
	glDeleteVertexArrays(1, &m_vao);
}


void FractalRender::setProgram(glpp::ProgramPtr program)
{
	m_program = program;
}


void FractalRender::doRender()
{
	glUseProgram(*m_program);

	m_program->setUniform("camera", { m_camera.x, m_camera.y });
	m_program->setUniform("zoom",   { m_zoom } );
	m_program->setUniform("screen", { m_screenDimension.x, m_screenDimension.y });
	m_program->setUniform("maxIterCount", { float(m_maxIters) });

	glBindTexture(GL_TEXTURE_1D, *m_texture);
	glBindVertexArray(m_vao);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
}

void FractalRender::onKeyPressed(int key, int scancode, int action, int mode)
{
	if (action == GLFW_RELEASE)
		return;

	switch (key) {
		case GLFW_KEY_EQUAL:
			m_maxIters = m_maxIters > 500 ? m_maxIters : m_maxIters + 5;
			break;
		case GLFW_KEY_MINUS:
			m_maxIters = m_maxIters < 50 ? m_maxIters : m_maxIters - 5;
			break;
	}
}

void FractalRender::onMouseMoved(double x, double y)
{
	if (m_mousePressed) {
		float dx = (x - m_lastPos.x) * m_zoom / m_screenDimension.x;
		float dy = (y - m_lastPos.y) * m_zoom / m_screenDimension.y;
		m_camera -= glm::vec2(dx, dy);
	}

	m_lastPos = glm::vec2(x, y);
}

void FractalRender::onMouseKeyPressed(int button, int action, int mods)
{
	m_mousePressed = (button == GLFW_MOUSE_BUTTON_LEFT) && (action == GLFW_PRESS);
}

void FractalRender::onMouseScroll(double xoff, double yoff)
{
	float mul = (yoff < 0) ? 1.1 : 0.9;
	auto d = m_lastPos / m_screenDimension;
	m_camera -= (d * m_zoom * (mul - 1));
	m_zoom *= mul;
}
