
#include "../Core/Core.h"

#include "RenderSystem.h"
#include "utility.h"

#include <string>
#include <iostream>
#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glm/glm.hpp>
//#include <glm/gtx/string_cast.hpp>
#include <cmath>

using namespace gl;
using namespace std;
using namespace core;

namespace renderSystem
{
	RenderSystem::RenderSystem()
	{
	}
	void RenderSystem::act()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Get uniform buffer
		glBindBuffer(GL_UNIFORM_BUFFER, frameDataUBO);
		auto data = reinterpret_cast<FrameData*>(glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(FrameData), GL_MAP_WRITE_BIT));

		// Update uniform data

		map<EntityType, LightComponent*>* lights = std::get<pComponentContainer(LightComponent)>(components);
		data->numLights = 0;
		for (auto it = lights->begin(); it != lights->end(); ++it)
			data->lights[data->numLights++] = it->second->light;

		auto cameras = std::get<pComponentContainer(CameraComponent)>(components);
		auto cameraSpot = cameras->begin();
		auto camera = cameraSpot->second;
		auto cameraEntity = cameraSpot->first;
		auto cameraPos = std::get<pComponentContainer(TransformComponent)>(components)->at(cameraEntity);

		// Quat to Mat4
		glm::mat4 identityMat = glm::mat4(1.0f);
		glm::mat4 rotMatrix = glm::mat4_cast(cameraPos->rotation);   //rotation is glm::quat
		glm::mat4 transMatrix = glm::translate(identityMat, cameraPos->position);
		glm::mat4 viewMatrix = rotMatrix * glm::inverse(transMatrix);

		data->projection = glm::perspective(camera->FOV, aspectRatio, camera->nearPlane, camera->farPlane);
		data->view = /*glm::lookAt(cameraPos->position, cameraPos->position + cameraPos->direction, camera->up);*/ viewMatrix;

		glUnmapBuffer(GL_UNIFORM_BUFFER); // Unmap uniform buffer

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
		mc->model.attachUniformBlock(Constants::get().getString("perframe_uniform_name"), Constants::get().getNum<int>("perframe_uniform_index"));

		return mc;
	}

	void RenderSystem::init()
	{
		FOV = Constants::get().getNum<float>("FOV");
		nearPlane = Constants::get().getNum<float>("near_plane");
		farPlane = Constants::get().getNum<float>("far_plane");
		windowWidth = Constants::get().getNum<int>("window_width");
		windowHeight = Constants::get().getNum<int>("window_height");
		perframeUniformName = Constants::get().getString("perframe_uniform_name");
		perframeUniformIndex = Constants::get().getNum<int>("perframe_uniform_index");
		
		ChangeManager::get().add(this, { Message::MsgType::RESIZE_WINDOW });
		LOG("Creating render system");

		// Load OpenGL Extensions
		glbinding::Binding::initialize();

		LOG("OpenGL Version: ", glGetString(GL_VERSION));
		LOG("Vendor: ", glGetString(GL_VENDOR));
		LOG("Renderer: ", glGetString(GL_RENDERER));
		LOG("GLSL Version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

		// Initialize OpenGL
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);

		aspectRatio = windowWidth / static_cast<float>(windowHeight);
		//glViewport(0, 0, 1, 1);
		
		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		//glClearColor(1, 1, 1, 1);

		// Create per-frame UBO
		glGenBuffers(1, &frameDataUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, frameDataUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(FrameData), NULL, GL_STREAM_DRAW);
		glBindBufferRange(GL_UNIFORM_BUFFER, perframeUniformIndex, frameDataUBO, 0, sizeof(FrameData));
	}
	
	void RenderSystem::recieveMsg(Message* msg)
	{
		switch (msg->type)
		{
		case Message::RESIZE_WINDOW:
			{
				auto m = dynamic_cast<ResizeWindowMessage*>(msg);
				auto width = m->width;
				auto height = m->height;
				aspectRatio = (height == 0) ? aspectRatio : static_cast<float>(width) / height;
				glViewport(0, 0, width, height);
			}
		}
	}
}