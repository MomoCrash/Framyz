#pragma once

#include "framework.h"

enum class CursorState {
	NORMAL 		= 0x00034001,
	HIDDEN 		= 0x00034002,
	DISABLED	= 0x00034003,
};

enum class InputMode {
	CURSOR					= 0x00033001,
	STICKY_KEYS				= 0x00033002,
	STICKY_MOUSE_BUTTONS	= 0x00033003,
	LOCK_KEY_MODS			= 0x00033004,
	RAW_MOUSE_MOTION		= 0x00033005,
};

class Window {

public:
	Window(const char* title, const int width, const int height);
	virtual ~Window();

	GLFWwindow* GetWindow();

	void SetInputMode(InputMode mode, CursorState state);

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