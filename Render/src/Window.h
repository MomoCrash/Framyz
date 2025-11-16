#pragma once

#include "framework.h"

class Window {

public:
	Window(const char* title, const int width, const int height);
	virtual ~Window();

	GLFWwindow* GetWindow();

	static void SetKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
	static void SetCharCallback(GLFWwindow* window, unsigned int c);
	static void SetMouseButtonCallback(GLFWwindow *window, int button, int action, int mode);
	static void SetScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void SetFocusCallback(GLFWwindow* window, int focused);


protected:

	GLFWwindow* m_window;
	const char* m_title;
	const int m_width;
	const int m_height;

};