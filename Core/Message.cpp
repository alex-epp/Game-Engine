#include "Message.h"

namespace core
{
	UpdateRenderableMessage::UpdateRenderableMessage(string h, string t, EntityType e) : header(h), text(t), entityID(e), Message(UPDATE_RENDERABLE)
	{
		type = UPDATE_RENDERABLE;
	}
	KeyChangeMessage::KeyChangeMessage(int k, bool d) : key(k), down(d), Message(KEY_CHANGE)
	{
		type = KEY_CHANGE;
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
			for (Listener *listener : listeners[messageQueue.front()->type])
				listener->recieveMsg(messageQueue.front());
			delete messageQueue.front();
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