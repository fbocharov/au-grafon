#ifndef BRICKS_RENDER_H
#define BRICKS_RENDER_H

#include <glpp/render.h>
#include <glpp/camera.h>
#include <glpp/texture.h>

#include <glm/vec2.hpp>

class BricksRender : public glpp::Render
{
public:
	BricksRender(int width, int height,
		std::string const & diffusePath, std::string const & normalPath,
		std::string const & heightPath);

	void setProgram(glpp::ProgramPtr program);

private:
	std::vector<GLfloat> prepareQuad();
	void moveCamera(double timeDelta);
	void RenderQuad();

private:
	void doRender() override;

	void onKeyPressed(int key, int scancode, int action, int mode) override;
	void onMouseMoved(double x, double y) override;
	void onMouseKeyPressed(int button, int action, int mods) override;
	void onMouseScroll(double xoff, double yoff) override;

private:
	glpp::ProgramPtr m_program;
	GLuint m_vao;
	GLuint m_vbo;

	glpp::TexturePtr m_diffuseMap;
	glpp::TexturePtr m_normalMap;
	glpp::TexturePtr m_heightMap;

	glm::vec3 m_lightPos = {0.5f, 1.0f, 0.3f};

	glpp::Camera m_camera;
	glm::vec2 m_lastPos;
	bool m_pressedKeys[512];
	double m_lastTimestamp;
};

#endif // BRICKS_RENDER_H
