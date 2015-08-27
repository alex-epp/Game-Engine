#pragma once

#include <string>
#include <map>
#include <list>
#include <queue>
#include <vector>

#include "Types.h"

using namespace std;

namespace core
{
	class Message
	{
	public:
		enum MsgType
		{
			UPDATE_RENDERABLE = 0,
			KEY_CHANGE,
			WINDOW_CLOSE,
			NUM_MSGS
		};

		MsgType type;
		virtual ~Message();
	protected:
		Message(MsgType);

	};

	class UpdateRenderableMessage : public Message
	{
	public:
		UpdateRenderableMessage(string h, string t, EntityType entityID);

		string header, text;
		EntityType entityID;
	};

	class KeyChangeMessage : public Message
	{
	public:
		KeyChangeMessage(int k, bool d);

		int key;
		bool down;
	};

	class WindowCloseMessage : public Message
	{
	public:
		WindowCloseMessage() : Message(WINDOW_CLOSE) {}
	};

	class Listener
	{
	public:
		virtual void recieveMsg(Message*) = 0;
	};

	class ChangeManager
	{
	private:
		ChangeManager() = default;

	public:
		static ChangeManager &get()
		{
			static ChangeManager instance;

			return instance;
		}

		void add(Listener*, list<Message::MsgType>);
		template <class Type, class... Args>
		void recieveMsg(Args&&... args);
		void distrubuteMsgs();

	private:
		map<Message::MsgType, std::vector<Listener*>> listeners;
		queue<Message*> messageQueue;
	};

	template <class Type, class... Args>
	void ChangeManager::recieveMsg(Args&&... args)
	{
		messageQueue.push(new Type(std::forward<Args>(args)...));
	}
}