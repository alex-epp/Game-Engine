
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

	void RenderSystem::addComponent(EntityType e, RenderableComponent* rc)
	{
		components[e] = rc;
	}

	void RenderSystem::act()
	{
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
				components[m->entityID]->header = m->header;
				components[m->entityID]->text = m->text;
			}
		}
	}
}