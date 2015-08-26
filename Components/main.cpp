#include "../RenderSystem/RenderSystem.h"
#include <iostream>

#ifdef _DEBUG
#pragma comment (lib, "../Debug/RenderSystem.lib")
#pragma comment (lib, "../Debug/Core.lib")
#else
#pragma comment (lib, "../Release/RenderSystem.lib")
#pragma comment (lib, "../Release/Core.lib")
#endif

using namespace std;
using namespace renderSystem;

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


int main()
{
	/*// Create a new entity
	ComponentManager::get().addComponent<LightComponent>(0);

	// Create another entity
	ComponentManager::get().addComponent<LightComponent>(1);
	*/

	// Create a render system
	RenderSystem rs;
	rs.init(&ComponentManager::get().getComponentsByType<LightComponent> ());

	// Send a message
	ChangeManager::get().recieveMsg<UpdateRenderableMessage>("Hello", "World", 0);
	ChangeManager::get().distrubuteMsgs();

	// Execute the render logic
	rs.act();

	// Cleanup the objects in memory
	ComponentManager::get().cleanup();

	cin.get();
}