
#include "../Core/Core.h"

#include "RenderSystem.h"
#include "utility.h"

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/GL.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


using namespace std;
using namespace core;

namespace renderSystem
{
	const float RenderSystem::FOV = 45.f;
	const float RenderSystem::NEAR_PLANE = 0.1f;
	const float RenderSystem::FAR_PLANE = 10'000.f;
	const int RenderSystem::WINDOW_WIDTH = 640;
	const int RenderSystem::WINDOW_HEIGHT = 480;
	const char* RenderSystem::FRAME_UNIFORM_NAME = "FrameData";
	int RenderSystem::FRAME_UNIFORM_INDEX;

	RenderSystem::RenderSystem()
	{
	}
	void RenderSystem::act()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		map<EntityType, LightComponent*>* lights = std::get<pComponentContainer(LightComponent)>(components);
		frameData.numLights = 0;
		for (auto it = lights->begin(); it != lights->end(); ++it)
			frameData.lights[frameData.numLights++] = it->second->light;

		frameData.projection = glm::perspective(FOV, WINDOW_WIDTH / static_cast<float>(WINDOW_HEIGHT), NEAR_PLANE, FAR_PLANE);
		frameData.view = glm::lookAt(vec3(0, 600, -800),
								vec3(0, 0, 0),
								vec3(0, 1, 0));

		updateFrameData();

		map<EntityType, ModelComponent*>* models = std::get<pComponentContainer(ModelComponent)>(components);
		for (auto it = models->begin(); it != models->end(); ++it)
		{
			it->second->model.render();
		}
	}

	ModelComponent* RenderSystem::createModel(string path, string filename)
	{
		LOG("Loading model from: ", filename);
		auto mc = new ModelComponent();
		mc->model.loadFromFile(path, filename);
		mc->model.attachUniformBlock(::Constants::PERFRAME_UNIFORM_NAME, ::Constants::PERFRAME_UNIFORM_INDEX);

		return mc;
	}

	void RenderSystem::init()
	{
		LOG("Creating render system");

		ChangeManager::get().add(this, { Message::MsgType::UPDATE_RENDERABLE });

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

		glViewport(0, 0, ::Constants::WINDOW_WIDTH, ::Constants::WINDOW_HEIGHT);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		//glClearColor(1, 1, 1, 1);

		// Create per-frame UBO
		glGenBuffers(1, &frameDataUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, frameDataUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(FrameData), NULL, GL_STREAM_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, ::Constants::PERFRAME_UNIFORM_INDEX, frameDataUBO, 0, sizeof(FrameData));

		// Some other book-keeping
		frameData.numLights = 0;
	}

	void RenderSystem::updateFrameData()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, frameDataUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(FrameData), &frameData);
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