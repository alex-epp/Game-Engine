#pragma once

#include <string>
#include "../Core/Core.h"

using namespace std;
using namespace core;

namespace renderSystem
{
	class RenderableComponent
	{
	public:
		string header;
		string text;
	};

	class RenderSystem : System<RenderableComponent>
	{
	public:
		RenderSystem();

		virtual void addComponent(EntityType, RenderableComponent*);
		virtual void act();
		virtual void recieveMsg(Message*);
	};
}