#include "Core.h"

namespace core
{
	UpdateRenderableMessage::UpdateRenderableMessage(string h, string t, EntityType e) : header(h), text(t), entityID(e), Message(UPDATE_RENDERABLE)
	{
		type = UPDATE_RENDERABLE;
	}

	void ChangeManager::add(Listener *o, list<Message::MsgType> l)
	{
		for (auto it = l.begin(); it != l.end(); ++it)
			listeners[*it].push_back(o);
	}

	void ChangeManager::distrubuteMsgs()
	{
		while (!messageQueue.empty())
		{
			for (Listener *listener : listeners[messageQueue.back()->type])
				listener->recieveMsg(messageQueue.back());
			delete messageQueue.back();
			messageQueue.pop();
		}
	}

	

	Message::Message(MsgType t) : type(t)
	{
	}
	Message::~Message()
	{
	}
}