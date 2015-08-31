#include "WindowContext.h"
#include "../Core/Logger.h"
#include <GLFW/glfw3.h>
#include "../Core/Constants.h"
using core::Constants;

namespace windowContext
{
	WindowContext::WindowContext()
	{
	}

	WindowContext::~WindowContext()
	{
	}

	void WindowContext::init()
	{
		LOG("Creating window context");

		// Initialize GLFW
		if (!glfwInit())
		{
			LOG_ERR("Could not initialize GLFW");
			return;
		}
		// Multisampling
		glfwWindowHint(GLFW_SAMPLES, 8);

		// Create a window
		window = glfwCreateWindow(Constants::get().getNum<int>("window_width"), Constants::get().getNum<int>("window_height"), Constants::get().getString("window_title").c_str(), nullptr, nullptr);
		if (!window)
		{
			LOG_ERR("Could not create GLFW window");
			return;
		}
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		glfwSetWindowUserPointer(window, this);
		quit = false;

		// Initialize callbacks
		//glfwSetMouseButtonCallback(window, mouseBtnCallback);
		//glfwSetCursorPosCallback(window, cursorPosCallback);
		//glfwSetCursorEnterCallback(window, cursorEnterCallback);
		//glfwSetScrollCallback(window, scrollCallback);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetWindowCloseCallback(window, closeCallback);
	}

	bool WindowContext::shouldQuit()
	{
		return quit || glfwWindowShouldClose(window);
	}

	void WindowContext::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			reinterpret_cast<WindowContext*>(glfwGetWindowUserPointer(window))->quit = true;
			ChangeManager::get().recieveMsg<WindowCloseMessage>();
		}

		if(action != GLFW_REPEAT)
			ChangeManager::get().recieveMsg<KeyChangeMessage>(key, action == GLFW_PRESS ? true : false);
	}

	void WindowContext::closeCallback(GLFWwindow * window)
	{
		reinterpret_cast<WindowContext*>(glfwGetWindowUserPointer(window))->quit = true;
		ChangeManager::get().recieveMsg<WindowCloseMessage>();
	}

	void WindowContext::cleanup()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void WindowContext::swapBuffers()
	{
		glfwSwapBuffers(window);
	}

	void WindowContext::act()
	{
		glfwPollEvents();
	}
}