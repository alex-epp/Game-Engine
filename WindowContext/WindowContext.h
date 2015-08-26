#pragma once
#include <GLFW/glfw3.h>
#include <string>

class WindowContext
{
public:
	WindowContext();
	~WindowContext();


	static void mouseBtnCallback(GLFWwindow* window, int button, int action, int mods);
	static void cursorPosCallback(GLFWwindow* window, double x, double y);
	static void cursorEnterCallback(GLFWwindow* window, int entered);
	static void scrollCallback(GLFWwindow* window, double x, double y);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static const int WINDOW_WIDTH;
	static const int WINDOW_HEIGHT;
	static const char* WINDOW_TITLE;
private:
	static WindowContext* inst;
	GLFWwindow* window;
};