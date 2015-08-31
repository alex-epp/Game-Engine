#pragma once


#include "../Core/Core.h"

#include "Model.h"

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <gl/GL.h>

using namespace std;
using namespace core;
using glm::vec4;
using glm::vec3;

// TODO: Add the following components:
// * Lights. These should store colour, radius, etc
// * Meshes. These should store geometry, shaders, etc
// * Cameras. These should store FOV, etc
// Integrate these into the RenderSystem,
// using code from OpenGL Engine.
// Also, the following should be added to Core:
// * Position
// * Orientation
// in order to complete the integration

namespace renderSystem
{
	struct Light // Designed to be passed into shaders (std140)
	{
		vec4 ambient, diffuse, specular;
		vec3 position;
		float radius;
	};

	// RenderSystem components
	struct LightComponent
	{
		Light light;
	};

	struct ModelComponent
	{
		Model model;
	};

	struct CameraComponent
	{
		// Stuff
	};

	struct FrameData
	{
		mat4 view;
		mat4 projection;

		float time;

		int numLights; vec2 filler;
		vector<Light> lights;
	};

	class RenderSystem : public System<pComponentContainer(LightComponent), pComponentContainer(ModelComponent)>
	{
	public:
		RenderSystem();

		virtual void recieveMsg(Message*);
		virtual void act();

		static ModelComponent* createModel(string, string);

		static LightComponent* createLight(string filename)
		{
			LOG("Loading light from: ", filename);

			ifstream file(filename.c_str());
			if (!file)
			{
				LOG_ERR("Could not open light from ", filename);
				return nullptr;
			}
			auto lightComponent = new LightComponent();
			auto& l = lightComponent->light;

			file >> l.ambient.r >> l.ambient.g >> l.ambient.b >> l.ambient.a;
			file >> l.diffuse.r >> l.diffuse.g >> l.diffuse.b >> l.diffuse.a;
			file >> l.specular.r >> l.specular.g >> l.specular.b >> l.specular.a;
			file >> l.position.x >> l.position.y >> l.position.z;
			file >> l.radius;

			file.close();

			return lightComponent;
		}
		
		void init();

		float FOV;
		float nearPlane;
		float farPlane;
		int windowWidth;
		int windowHeight;
		string perframeUniformName;
		int perframeUniformIndex;

	private:
		void updateFrameData();

		GLuint frameDataUBO;
		FrameData frameData;
	};
}