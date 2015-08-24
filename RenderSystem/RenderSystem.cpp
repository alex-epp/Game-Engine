
#include "RenderSystem.h"

#include <string>
#include <iostream>
#include "../Core/Core.h"

using namespace std;
using namespace core;

namespace renderSystem
{
	RenderSystem::RenderSystem()
	{
		ChangeManager::get().add(this, {Message::MsgType::UPDATE_RENDERABLE});
		//ChangeManager::get();
	}

	void RenderSystem::act()
	{
		auto &components = ComponentManager::get().getComponentsByType<RenderableComponent>();
		for (auto it = components.begin(); it != components.end(); ++it)
			cout << it->second->header << endl << it->second->text << endl << endl;
	}

	void RenderSystem::recieveMsg(Message* msg)
	{
		switch (msg->type)
		{
		case Message::UPDATE_RENDERABLE:
			{
				auto m = dynamic_cast<UpdateRenderableMessage*>(msg);
				cout << "Updating... header=" << m->header << " text=" << m->text << endl;
				auto comp = ComponentManager::get().getComponent<RenderableComponent>(m->entityID);
				comp->header = m->header;
				comp->text = m->text;
			}
		}
	}
}