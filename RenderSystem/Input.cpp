#include "Input.h"
#include "GameEngine.h"

void Input::init(GameEngine* engine)
{
	this->engine = engine;
	initialized = true;
}
void Input::quit()
{
	initialized = false;
}

void Input::keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Input::resizeCallbackImpl(GLFWwindow* window, int width, int height)
{
	if (initialized)
		engine->resizeWindow(width, height);
}