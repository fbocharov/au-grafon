#ifndef MODEL_RENDER_H
#define MODEL_RENDER_H

#include <glpp/render.h>
#include <glpp/program.h>
#include <glpp/camera.h>
#include <glpp/model.h>
#include <glpp/texture.h>

#include <unordered_map>

#include <glm/vec2.hpp>

class ModelRender : public glpp::Render
{
public:
	ModelRender(int width, int height, std::string const & modelPath,
				std::vector<std::string> const & skyboxFaces);
	~ModelRender();

	void setModelProgram(glpp::ProgramPtr program);
	void setSkyboxProgram(glpp::ProgramPtr program);

private:
	void doRender() override;

	void onKeyPressed(int key, int scancode, int action, int mode) override;
	void onMouseMoved(double x, double y) override;
	void onMouseKeyPressed(int button, int action, int mods) override;
	void onMouseScroll(double xoff, double yoff) override;

	void moveCamera(double timeDelta);

private:
	glpp::ProgramPtr m_modelProgram;
	glpp::ProgramPtr m_skyboxProgram;
	glpp::Model m_model;
	GLuint m_skyboxVAO;
	GLuint m_skyboxVBO;
	glpp::TexturePtr m_skyboxTexture;

	glpp::Camera m_camera;
	glm::vec2 m_lastPos = { 0, 0 };
	bool m_pressedKeys[512];
	double m_lastTimestamp;
};

#endif // MODEL_RENDER_H
