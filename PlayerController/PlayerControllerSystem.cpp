#include "PlayerControllerSystem.h"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace playerControllerSystem
{
	PlayerControllerSystem::PlayerControllerSystem()
	{
	}

	void PlayerControllerSystem::init()
	{
		ChangeManager::get().add(this, { Message::MsgType::MOUSE, Message::MsgType::KEY_CHANGE });

		xLast = 0;
		yLast = 0;

		x = 0;
		y = 0;
	}

	void PlayerControllerSystem::recieveMsg(Message* msg)
	{
		switch (msg->type)
		{
		case Message::MOUSE:
		{
			auto m = dynamic_cast<MouseMessage*>(msg);
			x = m->x;
			y = m->y;
			break;
		}
		case Message::MsgType::KEY_CHANGE:
		{
			auto m = dynamic_cast<KeyChangeMessage*>(msg);
			cout << m->key << endl;
		}
		}
	}

	void PlayerControllerSystem::act()
	{
		auto controllers = std::get<pComponentContainer(PlayerControllerComponent)>(components);
		auto transforms  = std::get<pComponentContainer(TransformComponent)>(components);
		for (auto it = controllers->begin(); it != controllers->end(); ++it)
		{
			auto entity = it->first;
			auto transform = transforms->at(entity);
			transform->rotation = glm::rotate(transform->rotation, (x - xLast) * 0.0001f, glm::vec3(0, 1, 0));
			transform->rotation = glm::rotate(transform->rotation, (y - yLast) * 0.0001f, glm::vec3(1, 0, 0) * transform->rotation );
			transform->position -= glm::vec3(0, 0, 1) * transform->rotation;
			//transform->direction.y = 1;
		}

		xLast = x;
		yLast = y;
	}
}

