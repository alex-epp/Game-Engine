#include "WindowContext.h"
#include "../Core/Logger.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

WindowContext* WindowContext::inst = nullptr;

const int WindowContext::WINDOW_WIDTH = 640;
const int WindowContext::WINDOW_HEIGHT = 480;
const char* WindowContext::WINDOW_TITLE = "Window Title";

WindowContext::WindowContext()
{
	// Initialize inst for static callbacks
	if(inst == nullptr)
	{
		inst = new WindowContext();

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

		// Initialize GLEW
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			LOG_ERR("Could not initialize GLEW: ", glewGetErrorString(err));
			return;
		}
		if (!GLEW_VERSION_4_3)
			LOG_WARN("OpenGL 4.3 is not supported");

		// Initialize OpenGL
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);

		glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
		glClearColor(0.f, 0.f, 0.f, 1.f);

		// Initialize callbacks
		glfwSetMouseButtonCallback(window, mouseBtnCallback);
		glfwSetCursorPosCallback(window, cursorPosCallback);
		glfwSetCursorEnterCallback(window, cursorEnterCallback);
		glfwSetScrollCallback(window, scrollCallback);
		glfwSetKeyCallback(window, keyCallback);
	}
}

WindowContext::~WindowContext()
{
	glfwDestroyWindow(window);
}