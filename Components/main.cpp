#include "../RenderSystem/RenderSystem.h"
#include "../WindowContext/WindowContext.h"
#include "../PlayerController/PlayerControllerSystem.h"
#include <iostream>


#if defined(_WIN64) && defined(_DEBUG)
#define LIB_DIR "../x64/Debug/"
#elif defined(_WIN64)
#define LIB_DIR "../x64/Release/"
#elif defined(_DEBUG)
#define LIB_DIR "../Debug/"
#else
#define LIB_DIR "../Release/"
#endif

#pragma comment (lib, LIB_DIR "RenderSystem.lib")
#pragma comment (lib, LIB_DIR "Core.lib")
#pragma comment (lib, LIB_DIR "WindowContext.lib")
#pragma comment (lib, LIB_DIR "PlayerController.lib")


using namespace std;
using namespace renderSystem;
using namespace windowContext;
using namespace playerControllerSystem;

class ComponentManager
{
private:

	// This tuple must contain all the possible component types
	tuple<ComponentContainer(LightComponent),
		ComponentContainer(ModelComponent),
		ComponentContainer(CameraComponent),
		ComponentContainer(TransformComponent),
		ComponentContainer(PlayerControllerComponent)> componentLists;

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
		pcs.init();

		int ID = 0;
		auto modelPaths = Constants::get().getStringArray("model_paths");
		auto models = Constants::get().getStringArray("models");
		for (decltype(modelPaths.size()) i = 0; i < modelPaths.size(); ++i)
		{
			ComponentManager::get().addComponent(++ID, RenderSystem::createModel(modelPaths[i], models[i]));
		}
		auto lights = Constants::get().getStringArray("lights");
		for (auto light : lights)
		{
			ComponentManager::get().addComponent<LightComponent>(++ID, RenderSystem::createLight(light));
		}
		ComponentManager::get().addComponent<CameraComponent>(++ID, 45.f, glm::vec3(0, 1, 0), 0.1f, 10'000.f);
		ComponentManager::get().addComponent<TransformComponent>(ID, glm::vec3(0, 600, 0), glm::angleAxis(0.f, glm::vec3(0, 1, 0) ) );
		ComponentManager::get().addComponent<PlayerControllerComponent>(ID);

		quit = false;
		ChangeManager::get().add(this, { Message::WINDOW_CLOSE });

		rs.setComponents(&ComponentManager::get().getComponentsByType<LightComponent>(),
			&ComponentManager::get().getComponentsByType<ModelComponent>(),
			&ComponentManager::get().getComponentsByType<CameraComponent>(),
			&ComponentManager::get().getComponentsByType<TransformComponent>());

		pcs.setComponents(&ComponentManager::get().getComponentsByType<TransformComponent>(),
			&ComponentManager::get().getComponentsByType<PlayerControllerComponent>());
	}

	void loop()
	{
		while (!quit)
		{
			// Execute the render logic
			rs.act();
			pcs.act();
			wc.swapBuffers();
			wc.act();
			ChangeManager::get().distrubuteMsgs();

			FPSCounter::update();
		}
	}

	~GameEngine()
	{
		// Cleanup the objects in memory
		ComponentManager::get().cleanup();
		core::Constants::get().cleanup();
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
	PlayerControllerSystem pcs;
	WindowContext wc;
};

int main()
{
	GameEngine().loop();
}