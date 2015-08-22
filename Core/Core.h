#pragma once

#include <map>
#include <queue>
#include <list>
#include <vector>
#include <string>

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

	template <typename ComponentType>
	class System : public Listener
	{
	protected:
		map<EntityType, ComponentType*> components;
		time_t nextAct;

	public:
		virtual void addComponent(EntityType, ComponentType*) = 0;
		virtual void act() = 0;
	};


}