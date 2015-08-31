#include "PlayerControllerSystem.h"

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <GLFW/glfw3.h>

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

		playerForward = playerBackward = playerLeft = playerRight = false;
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
			switch (m->key)
			{
			case GLFW_KEY_W:
				playerForward = m->down;
				break;
			case GLFW_KEY_S:
				playerBackward = m->down;
				break;
			case GLFW_KEY_A:
				playerLeft = m->down;
				break;
			case GLFW_KEY_D:
				playerRight = m->down;
				break;
			}
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
			transform->rotation = glm::rotate(transform->rotation, (x - xLast) * 0.001f, glm::vec3(0, 1, 0));
			transform->rotation = glm::rotate(transform->rotation, (y - yLast) * 0.001f, glm::vec3(1, 0, 0) * transform->rotation );
			if (playerForward)
				transform->position -= (glm::vec3(0, 0, 1) * transform->rotation) * 3.f;
			if (playerBackward)
				transform->position += (glm::vec3(0, 0, 1) * transform->rotation) * 3.f;
			if (playerLeft)
				transform->position -= (glm::vec3(1, 0, 0) * transform->rotation) * 3.f;
			if (playerRight)
				transform->position += (glm::vec3(1, 0, 0) * transform->rotation) * 3.f;
			//transform->direction.y = 1;
		}

		xLast = x;
		yLast = y;
	}
}

