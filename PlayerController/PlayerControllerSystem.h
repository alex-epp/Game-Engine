#pragma once

#include "../Core/Core.h"

#include <string>

using namespace std;
using namespace core;

namespace playerControllerSystem
{
	struct PlayerControllerComponent
	{
		// Nothing for now
	};

	class PlayerControllerSystem : public System<pComponentContainer(TransformComponent),
		pComponentContainer(PlayerControllerComponent)>
	{
	public:
		PlayerControllerSystem();

		virtual void recieveMsg(Message*);
		virtual void act();

		void init();

	private:
		int x, y;
		int xLast, yLast;
	};
}