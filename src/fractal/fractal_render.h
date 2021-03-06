#ifndef FRACTAL_RENDER_H
#define FRACTAL_RENDER_H

#include <glpp/render.h>
#include <glpp/texture.h>

#include <glm/vec2.hpp>

#include <string>

class FractalRender : public glpp::Render
{
public:
	FractalRender(int width, int height, std::string const & texturePath);
	~FractalRender();

	void setProgram(glpp::ProgramPtr program);

private:
	void doRender() override;

	void onKeyPressed(int key, int scancode, int action, int mode) override;
	void onMouseMoved(double xpos, double ypos) override;
	void onMouseKeyPressed(int button, int action, int mods) override;
	void onMouseScroll(double xoff, double yoff) override;

private:
	glpp::ProgramPtr m_program;

	GLuint m_vao;
	glpp::TexturePtr m_texture;

	glm::vec2 m_camera = glm::vec2(-1.5, -1.0);
	float m_zoom = 2.0;
	int m_maxIters = 100;
	bool m_mousePressed = false;
	glm::vec2 m_lastPos;

	glm::vec2 const m_screenDimension;
};

#endif	// FRACTAL_RENDER_H
