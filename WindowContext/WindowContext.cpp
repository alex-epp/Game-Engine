#include "WindowContext.h"
#include "../Core/Logger.h"
#include <GLFW/glfw3.h>

namespace windowContext
{
	const int WindowContext::WINDOW_WIDTH = 640;
	const int WindowContext::WINDOW_HEIGHT = 480;
	const char* WindowContext::WINDOW_TITLE = "Window Title";

	WindowContext::WindowContext()
	{
		// Initialize GLFW
		if (!glfwInit())
		{
			LOG_ERR("Could not initialize GLFW");
			return;
		}
		// Multisampling
		glfwWindowHint(GLFW_SAMPLES, 8);

		// Create a window
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
		if (!window)
		{
			LOG_ERR("Could not create GLFW window");
			return;
		}
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1);
		quit = false;

		// Initialize callbacks
		//glfwSetMouseButtonCallback(window, mouseBtnCallback);
		//glfwSetCursorPosCallback(window, cursorPosCallback);
		//glfwSetCursorEnterCallback(window, cursorEnterCallback);
		//glfwSetScrollCallback(window, scrollCallback);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetWindowCloseCallback(window, closeCallback);
	}

	WindowContext &WindowContext::get()
	{
		static WindowContext w;
		return w;
	}

	bool WindowContext::shouldQuit()
	{
		return quit || glfwWindowShouldClose(window);
	}

	void WindowContext::keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			get().quit = true;
			ChangeManager::get().recieveMsg<WindowCloseMessage>();
		}

		if(action != GLFW_REPEAT)
			ChangeManager::get().recieveMsg<KeyChangeMessage>(key, action == GLFW_PRESS ? true : false);
	}

	void WindowContext::closeCallback(GLFWwindow * window)
	{
		get().quit = true;
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
		glfwPollEvents();
	}
}