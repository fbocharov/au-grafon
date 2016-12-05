#ifndef GLPP_RENDER_H
#define GLPP_RENDER_H

#include "program.h"

#include <cstddef>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

namespace glpp
{

class Render
{
public:
	Render(int width, int height, std::string const & name, bool disableCursor = false);
	virtual ~Render();

	bool isStopped() const;
	void render();

private:
	void setupCallbacks();
	static void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mode);
	static void mouseMoveCallback(GLFWwindow * window, double xpos, double ypos);
	static void mouseKeyCallback(GLFWwindow * window, int button, int action, int mods);
	static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
	virtual void doRender() = 0;

	virtual void onKeyPressed(int key, int scancode, int action, int mode) = 0;
	virtual void onMouseMoved(double xpos, double ypos) = 0;
	virtual void onMouseKeyPressed(int button, int action, int mods) = 0;
	virtual void onMouseScroll(double xoffset, double yoffset) = 0;

protected:
	glm::vec2 const m_dimension;

private:
	GLFWwindow * m_window;
};

}	// namespace glpp

#endif	// GLPP_RENDER_H
