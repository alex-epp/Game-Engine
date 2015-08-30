#include "Message.h"

#include "Types.h"

namespace core
{
	ResizeWindowMessage::ResizeWindowMessage(int w, int h) : width(w), height(h), Message(RESIZE_WINDOW)
	{
	}
	KeyChangeMessage::KeyChangeMessage(int k, bool d) : key(k), down(d), Message(KEY_CHANGE)
	{
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