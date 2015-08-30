#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include "../Core/Core.h"
using namespace core;

namespace windowContext
{
	class Callbacks;
	
	class WindowContext : public Listener
	{
	public:
		//static WindowContext &get();
		WindowContext();
		~WindowContext();

		void init();
		bool shouldQuit();
		void cleanup();
		void swapBuffers();
		void act();

		virtual void recieveMsg(Message*);

		// Constants
		static const int WINDOW_WIDTH;
		static const int WINDOW_HEIGHT;
		static const char* WINDOW_TITLE;

	private:

		// Input callbacks
		//static void mouseBtnCallback(GLFWwindow* window, int button, int action, int mods);
		static void cursorPosCallback(GLFWwindow* window, double x, double y);
		//static void cursorEnterCallback(GLFWwindow* window, int entered);
		//static void scrollCallback(GLFWwindow* window, double x, double y);
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void closeCallback(GLFWwindow* window);
		static void sizeCallback(GLFWwindow* window, int width, int height);

		GLFWwindow* window;

		bool quit;
	};
}