#include "../RenderSystem/RenderSystem.h"
#include "../WindowContext/WindowContext.h"
#include <iostream>

#ifdef _DEBUG
#pragma comment (lib, "../Debug/RenderSystem.lib")
#pragma comment (lib, "../Debug/Core.lib")
#pragma comment (lib, "../Debug/WindowContext.lib")
#else
#pragma comment (lib, "../Release/RenderSystem.lib")
#pragma comment (lib, "../Release/Core.lib")
#pragma comment (lib, "../Release/WindowContext.lib")
#endif

using namespace std;
using namespace renderSystem;
using namespace windowContext;

class ComponentManager
{
private:

	// This tuple must contain all the possible component types
	tuple<ComponentContainer(LightComponent), ComponentContainer(ModelComponent)> componentLists;
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

	template<class ComponentType>
	void addComponent(EntityType objectID, ComponentType* component)
	{
		std::get<map<EntityType, ComponentType*>>(componentLists)[objectID] = component;
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

class GameEngine : public Listener
{
public:
	GameEngine()
	{
		wc.init();
		rs.init();

		ComponentManager::get().addComponent<ModelComponent>(0, RenderSystem::createModel("../data/models/crytek-sponza/", "sponza2.obj"));

		quit = false;
		ChangeManager::get().add(this, { Message::WINDOW_CLOSE });

		rs.setComponents(&ComponentManager::get().getComponentsByType<LightComponent>(), &ComponentManager::get().getComponentsByType<ModelComponent>());
	}

	void loop()
	{
		while (!quit)
		{
			// Execute the render logic
			rs.act();
			wc.swapBuffers();
			wc.act();
			ChangeManager::get().distrubuteMsgs();
		}
	}

	~GameEngine()
	{
		// Cleanup the objects in memory
		ComponentManager::get().cleanup();
	}

	virtual void recieveMsg(Message* msg)
	{
		switch (msg->type)
		{
		case Message::WINDOW_CLOSE:
			quit = true;
		}
	}

private:
	bool quit;
	RenderSystem rs;
	WindowContext wc;
};

int main()
{
	GameEngine().loop();
}