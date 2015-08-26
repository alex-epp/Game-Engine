#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Constants.h"
#include <memory>

class RenderSystem;

// This class must be a singleton because GLFW requires c-style callbacks
class Input
{
public:
	static Input& getInstance() // Get singleton instance
	{
		static Input instance;
		return instance;
	}
	void init(RenderSystem* engine); // Initialize the singleton
	void quit();

	// GLFW callbacks

	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		getInstance().mouseButtonCallbackImpl(window, button, action, mods);
	}
	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		getInstance().cursorPosCallbackImpl(window, xpos, ypos);
	}
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		getInstance().keyCallbackImpl(window, key, scancode, action, mods);
	}
	static void resizeCallback(GLFWwindow* window, int width, int height)
	{
		getInstance().resizeCallbackImpl(window, width, height);
	}

	// Helper functions

	void mouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods);
	void cursorPosCallbackImpl(GLFWwindow* window, double xpos, double ypos);
	void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);
	void resizeCallbackImpl(GLFWwindow* window, int width, int height);

	Input(const Input&) = delete;
	Input(const Input&&) = delete;
	void operator=(const Input&) = delete;
	void operator=(const Input&&) = delete;

private:
	Input()
	{
		// OpenGL and GLFW should be initialized by now
		initialized = false;
	}

	RenderSystem* engine;
	bool initialized; // Needed to check whether 'engine' is valid
};

