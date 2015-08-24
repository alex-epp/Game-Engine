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

int main()
{
	// Create a new entity
	ComponentManager::get().addComponent<RenderableComponent>(0, "Hello World", "This is a component");

	// Create another entity
	ComponentManager::get().addComponent<RenderableComponent>(1, "Another Header", "Blah blah blah blah blah blah blah blah");

	// Create a render system
	RenderSystem rs;
	
	// Send a message
	ChangeManager::get().recieveMsg<UpdateRenderableMessage>("Hello", "World", 0);
	ChangeManager::get().distrubuteMsgs();

	// Execute the render logic
	rs.act();

	// Cleanup the objects in memory
	ComponentManager::get().cleanup();

	cin.get();
}