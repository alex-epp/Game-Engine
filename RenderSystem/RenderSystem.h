#pragma once

#include <string>
#include "../Core/Core.h"

using namespace std;
using namespace core;

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
	class RenderSystem : System
	{
	public:
		RenderSystem();

		virtual void act();
		virtual void recieveMsg(Message*);
	};
}