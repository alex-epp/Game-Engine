#pragma once

#include <map>
#include <queue>
#include <list>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>
#include "utility.h"
#include <glm/glm.hpp>
#include <gl/GL.h>
using glm::vec3;
using glm::vec4;

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

	class System : public Listener
	{
	protected:
		time_t nextAct;

	public:
		virtual void act() = 0;
	};

	// RenderSystem components
	class LightComponent // Designed to be passed into shaders (std140)
	{
		vec4 ambient, diffuse, specular;
		vec3 position;
		float radius;
	};

	class ComponentManager
	{
	private:

		// This tuple must contain all the possible component types
		tuple<map<EntityType, LightComponent*>> componentLists;
		ComponentManager() = default;

	public:

		static ComponentManager &get()
		{
			static ComponentManager instance;

			return instance;
		}


		template<class ComponentType, class... Args>
		void addComponent(EntityType objectID, Args&&... args)
		{
			auto &theMap = std::get<map<EntityType, ComponentType*>>(componentLists);
			theMap[objectID] = new ComponentType(std::forward<Args>(args)...);

		}
		
		template <class ComponentType>
		auto &getComponent(EntityType entity)
		{
			return std::get<map<EntityType, ComponentType*>>(componentLists)[entity];
		}

		template <class ComponentType>
		auto &getComponentsByType()
		{
			return std::get<map<EntityType, ComponentType*>>(componentLists);
		}

		void cleanup()
		{
			for_each(componentLists, [](auto &m)
			{
				for (auto it = m.begin(); it != m.end(); ++it)
					delete it->second;
			});
		}
	};
}