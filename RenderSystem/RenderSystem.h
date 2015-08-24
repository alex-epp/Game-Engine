#pragma once

#include <string>
#include "../Core/Core.h"

using namespace std;
using namespace core;

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