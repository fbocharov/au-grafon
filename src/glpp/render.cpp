#include "render.h"

#include <stdexcept>
#include <cassert>
#include <functional>

namespace glpp
{

Render::Render(int width, int height, std::string const & name)
{
	if (GL_TRUE != glfwInit())
		throw std::runtime_error("failed to initialize GLFW");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
	if (!m_window)
		throw std::runtime_error("failed to create GLFW context");
	glfwMakeContextCurrent(m_window);

	setupCallbacks();

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		glfwTerminate();
		throw std::runtime_error("failed to initialize GLEW");
	}

	glViewport(0, 0, width, height);
}

Render::~Render()
{
	glfwTerminate();
}

bool Render::isStopped() const
{
	return glfwWindowShouldClose(m_window);
}

void Render::render(ProgramPtr program)
{
	glUseProgram(*program);
	doRender(program);
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void Render::setupCallbacks()
{
	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, keyCallback);
	glfwSetCursorPosCallback(m_window, mouseMoveCallback);
	glfwSetMouseButtonCallback(m_window, mouseKeyCallback);
	glfwSetScrollCallback(m_window, mouseScrollCallback);
}

void Render::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	reinterpret_cast<Render *>(glfwGetWindowUserPointer(window))->onKeyPressed(key, scancode, action, mode);
}

void Render::mouseMoveCallback(GLFWwindow * window, double xpos, double ypos)
{
	reinterpret_cast<Render *>(glfwGetWindowUserPointer(window))->onMouseMoved(xpos, ypos);
}

void Render::mouseKeyCallback(GLFWwindow * window, int button, int action, int mods)
{
	reinterpret_cast<Render *>(glfwGetWindowUserPointer(window))->onMouseKeyPressed(button, action, mods);
}

void Render::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	reinterpret_cast<Render *>(glfwGetWindowUserPointer(window))->onMouseScroll(xoffset, yoffset);
}

}	// namespace glpp
