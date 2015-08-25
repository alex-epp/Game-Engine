
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
		auto lights = std::get<ComponentContainer(LightComponent)>(components);
		for (auto it = lights->begin(); it != lights->end(); ++it)
			cout << it->second->radius << endl << endl;
	}

	
	void RenderSystem::recieveMsg(Message* msg)
	{
		/*switch (msg->type)
		{
		case Message::UPDATE_RENDERABLE:
			{
				auto m = dynamic_cast<UpdateRenderableMessage*>(msg);
				cout << "Updating... header=" << m->header << " text=" << m->text << endl;
				auto comp = ComponentManager::get().getComponent<RenderableComponent>(m->entityID);
				comp->header = m->header;
				comp->text = m->text;
			}
		}*/
	}
}