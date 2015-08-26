
#include "RenderSystem.h"

#include <string>
#include <iostream>
#include "../Core/Core.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include "Input.h"

using namespace std;
using namespace core;

namespace renderSystem
{
	RenderSystem::RenderSystem()
	{
		ChangeManager::get().add(this, {Message::MsgType::UPDATE_RENDERABLE});
		
		// Initialize glfw
		if (!glfwInit())
		{
			LOG_ERR("Could not initialize glfw");
			return;
		}
		// Multisampling
		glfwWindowHint(GLFW_SAMPLES, 8);

		GLFWmonitor* moniter = Constants::FULLSCREEN ? glfwGetPrimaryMonitor() : NULL;

		window = glfwCreateWindow(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT, Constants::WINDOW_TITLE, moniter, NULL);
		if (!window)
		{
			LOG_ERR("Could not create glfw window");
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
		LOG("GLEW Version: ", GLEW_VERSION);
		LOG("OpenGL Version: ", glGetString(GL_VERSION));
		LOG("Vendor: ", glGetString(GL_VENDOR));
		LOG("Renderer: ", glGetString(GL_RENDERER));
		LOG("GLSL Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

		if (!GLEW_VERSION_4_3)
			LOG_WARN("OpenGL 4.3 is not supported");


		// Initialize OpenGL
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);

		glViewport(0, 0, Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		//glClearColor(1, 1, 1, 1);

		// Create Input singleton
		Input& inst = Input::getInstance(); // The first time getInstance() is called
		inst.init(this);
		glfwSetKeyCallback(window, Input::keyCallback);
		glfwSetWindowSizeCallback(window, Input::resizeCallback);

		// Create per-frame UBO
		glGenBuffers(1, &frameDataUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, frameDataUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(FrameData), NULL, GL_STREAM_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, Constants::PERFRAME_UNIFORM_INDEX, frameDataUBO, 0, sizeof(FrameData));

		// Some other book-keeping
		frameData.numLights = 0;
	}
	void RenderSystem::act()
	{
		auto lights = std::get<ComponentContainer(LightComponent)>(components);
		for (auto it = lights->begin(); it != lights->end(); ++it)
			cout << it->second->light.radius << endl
			     << glm::to_string(it->second->light.ambient) << endl
				 << glm::to_string(it->second->light.diffuse) << endl
				 << glm::to_string(it->second->light.position) << endl
				 << glm::to_string(it->second->light.specular) << endl
				 << endl;
	}

	
	void RenderSystem::recieveMsg(Message* msg)
	{
		/*switch (msg->type)
		{
		case Message::UPDATE_RENDERABLE:
			{
				auto m = dynamic_cast<UpdateRenderableMessage*>(msg);
				cout << "Updating... header=" << m->header << " text=" << m->text << endl;
				auto comp = ComponentManager::get().getComponent<RenderableComponent>(m->entityID);
				comp->header = m->header;
				comp->text = m->text;
			}
		}*/
	}
}