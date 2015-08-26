#pragma once

#include <map>
#include <queue>
#include <list>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include "utility.h"

using namespace std;

namespace core
{
	typedef unsigned int EntityType;

	// *****************************************************************************************************************
	// Event Manager
	// *****************************************************************************************************************

	class Message
	{
	public:
		enum MsgType
		{
			UPDATE_RENDERABLE = 0,
			KEY_CHANGE,
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

	// *****************************************************************************************************************
	// System
	// *****************************************************************************************************************

	template <class... ComponentsLists>
	class System : public Listener
	{
	protected:
		time_t nextAct;
		tuple<ComponentsLists...> components = {  };

	public:
		void init(ComponentsLists... c)
		{
			components = std::forward_as_tuple(c...);
		}
		virtual void act() = 0;
	};

#define ComponentContainer(T) map<EntityType, T*>*
}