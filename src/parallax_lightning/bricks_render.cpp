#include "bricks_render.h"

#include <glpp/shader.h>
#include <glpp/camera.h>
#include <glpp/model.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

BricksRender::BricksRender(int width, int height, std::string const & diffusePath,
		std::string const & normalPath, std::string const & heightPath)
	: glpp::Render(width, height, "Parallax occlusion lightning", true)
	, m_diffuseMap(glpp::Texture::load2D(diffusePath))
	, m_normalMap(glpp::Texture::load2D(normalPath))
	, m_heightMap(glpp::Texture::load2D(heightPath))
	, m_camera(glm::vec3(0.0f, 0.0f, 3.0f))
{
	glEnable(GL_DEPTH_TEST);

	auto vertices = prepareQuad();
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));

}

void BricksRender::setProgram(glpp::ProgramPtr program)
{
	m_program = program;
	glUseProgram(*m_program);
	glUniform1i(glGetUniformLocation(*m_program, "diffuseMap"), 0);
	glUniform1i(glGetUniformLocation(*m_program, "normalMap"), 1);
	glUniform1i(glGetUniformLocation(*m_program, "depthMap"), 2);
}

void BricksRender::doRender()
{
	double currentFrame = glfwGetTime();
	double timeDelta = currentFrame - m_lastTimestamp;
	m_lastTimestamp = currentFrame;

	moveCamera(timeDelta);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(*m_program);
	glm::mat4 view = m_camera.getView();
	glm::mat4 projection = glm::perspective(m_camera.getZoom(), m_dimension.x / m_dimension.y, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(*m_program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(*m_program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 model;
	glUniformMatrix4fv(glGetUniformLocation(*m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(glGetUniformLocation(*m_program, "lightPos"), 1, &m_lightPos[0]);
	glUniform3fv(glGetUniformLocation(*m_program, "viewPos"), 1, &m_camera.getPosition()[0]);
	glUniform1f(glGetUniformLocation(*m_program, "height_scale"), 0.1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *m_diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *m_normalMap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, *m_heightMap);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	model = glm::mat4();
	model = glm::translate(model, m_lightPos);
	model = glm::scale(model, glm::vec3(0.1f));
	glUniformMatrix4fv(glGetUniformLocation(*m_program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void BricksRender::moveCamera(double timeDelta)
{
	if (m_pressedKeys[GLFW_KEY_W]) m_camera.move(glpp::Direction::FORWARD, timeDelta);
	if (m_pressedKeys[GLFW_KEY_S]) m_camera.move(glpp::Direction::BACKWARD, timeDelta);
	if (m_pressedKeys[GLFW_KEY_D]) m_camera.move(glpp::Direction::RIGHT, timeDelta);
	if (m_pressedKeys[GLFW_KEY_A]) m_camera.move(glpp::Direction::LEFT, timeDelta);
}

std::vector<GLfloat> BricksRender::prepareQuad()
{
	glm::vec3 pos1(-1.0, 1.0, 0.0);
	glm::vec3 pos2(-1.0, -1.0, 0.0);
	glm::vec3 pos3(1.0, -1.0, 0.0);
	glm::vec3 pos4(1.0, 1.0, 0.0);

	glm::vec2 uv1(0.0, 1.0);
	glm::vec2 uv2(0.0, 0.0);
	glm::vec2 uv3(1.0, 0.0);
	glm::vec2 uv4(1.0, 1.0);

	glm::vec3 nm(0.0, 0.0, 1.0);

	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;

	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);

	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);

	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent2 = glm::normalize(bitangent2);

	return {
		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};
}

void BricksRender::onKeyPressed(int key, int scancode, int action, int mode)
{
	m_pressedKeys[key] = (action != GLFW_RELEASE);
}

void BricksRender::onMouseMoved(double x, double y)
{
	GLfloat xoffset = x - m_lastPos.x;
	GLfloat yoffset = m_lastPos.y - y;

	m_lastPos = { x, y };

	m_camera.turn(xoffset, yoffset);
}

void BricksRender::onMouseKeyPressed(int button, int action, int mods)
{
	// do nothing
}

void BricksRender::onMouseScroll(double xoff, double yoff)
{
	m_camera.zoom(yoff / 10);
}
