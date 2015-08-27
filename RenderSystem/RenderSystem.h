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
	struct Light
	{
		vec4 ambient, diffuse, specular;
		vec3 position;
		float radius;
	};

	// RenderSystem components
	struct LightComponent // Designed to be passed into shaders (std140)
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
		Light lights[Constants::MAX_LIGHTS];
	};

	class RenderSystem : public System<pComponentContainer(LightComponent), pComponentContainer(ModelComponent)>
	{
	public:
		RenderSystem();

		virtual void recieveMsg(Message*);
		virtual void act();

		static ModelComponent* createModel(string, string);

		static auto createLight(string filename)
		{
			cout << "Loading light from: " << filename << endl;
			return new LightComponent();
		}

		void init();

	private:
		GLuint frameDataUBO;
		FrameData frameData;
	};
}