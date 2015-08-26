#pragma once

#include <string>
#include "../Core/Core.h"
#include "../Lib/glew-1.13.0/include/GL/glew.h"
#include "../Lib/glm/glm/glm.hpp"
#include <gl/GL.h>
#include "Model.h"

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
	// RenderSystem components
	struct LightComponent // Designed to be passed into shaders (std140)
	{
		vec4 ambient, diffuse, specular;
		vec3 position;
		float radius;
	};

	struct ModelComponent
	{
		Model model;
	};

	class RenderSystem : public System<ComponentContainer(LightComponent)>
	{
	public:
		RenderSystem();

		virtual void recieveMsg(Message*);
		virtual void act();
	};
}