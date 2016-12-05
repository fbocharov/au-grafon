#include "model_render.h"

#include <glm/mat3x3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{

const GLfloat SKYBOX[] =
{
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

}	// namespace

ModelRender::ModelRender(int width, int height, std::string const & modelPath,
		std::vector<std::string> const & skyboxFaces)
	: glpp::Render(width, height, "3D model", true)
	, m_model(modelPath)
	, m_skyboxTexture(glpp::Texture::loadCubemap(skyboxFaces))
	, m_camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &m_skyboxVAO);
	glBindVertexArray(m_skyboxVAO);

	glGenBuffers(1, &m_skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX), &SKYBOX, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
	glBindVertexArray(0);
}

ModelRender::~ModelRender()
{
	glDeleteVertexArrays(1, &m_skyboxVAO);
	glDeleteBuffers(1, &m_skyboxVBO);
}

void ModelRender::setModelProgram(glpp::ProgramPtr program)
{
	m_modelProgram = program;
}

void ModelRender::setSkyboxProgram(glpp::ProgramPtr program)
{
	m_skyboxProgram = program;
}

void ModelRender::doRender()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	double currentFrame = glfwGetTime();
	double deltaTime = currentFrame - m_lastTimestamp;
	m_lastTimestamp = currentFrame;

	moveCamera(deltaTime);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(*m_modelProgram);
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -1.f, 0.0f));
	model = glm::scale(model, glm::vec3(10.f, 10.f, 10.f));
	glm::mat4 view = m_camera.getView();
	glm::mat4 projection = glm::perspective(m_camera.getZoom(), m_dimension.x / m_dimension.y, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(*m_modelProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(*m_modelProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(*m_modelProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	auto camPos = m_camera.getPosition();
	m_modelProgram->setUniform("camera", { camPos.x, camPos.y, camPos.z });

	glBindTexture(GL_TEXTURE_CUBE_MAP, *m_skyboxTexture);
	m_model.render(m_skyboxProgram);

	glDepthFunc(GL_LEQUAL);
	glUseProgram(*m_skyboxProgram);
	view = glm::mat4(glm::mat3(m_camera.getView()));
	glUniformMatrix4fv(glGetUniformLocation(*m_skyboxProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(*m_skyboxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glBindVertexArray(m_skyboxVAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *m_skyboxTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

void ModelRender::onKeyPressed(int key, int scancode, int action, int mode)
{
	m_pressedKeys[key] = (action != GLFW_RELEASE);
}

void ModelRender::onMouseMoved(double x, double y)
{
	GLfloat xoffset = x - m_lastPos.x;
	GLfloat yoffset = m_lastPos.y - y;

	m_lastPos = { x, y };

	m_camera.turn(xoffset, yoffset);
}
void ModelRender::onMouseKeyPressed(int button, int action, int mods)
{
	// do nothing
}

void ModelRender::onMouseScroll(double xoff, double yoff)
{
	m_camera.zoom(yoff / 10);
}

void ModelRender::moveCamera(double timeDelta)
{
	if (m_pressedKeys[GLFW_KEY_W]) m_camera.move(glpp::Direction::FORWARD, timeDelta);
	if (m_pressedKeys[GLFW_KEY_S]) m_camera.move(glpp::Direction::BACKWARD, timeDelta);
	if (m_pressedKeys[GLFW_KEY_D]) m_camera.move(glpp::Direction::RIGHT, timeDelta);
	if (m_pressedKeys[GLFW_KEY_A]) m_camera.move(glpp::Direction::LEFT, timeDelta);
}
