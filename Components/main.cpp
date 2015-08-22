#include "../RenderSystem/RenderSystem.h"
#include <iostream>

#pragma comment (lib, "../Debug/RenderSystem.lib")
#pragma comment (lib, "../Debug/Core.lib")

using namespace std;
using namespace renderSystem;

int main()
{
	RenderSystem rs;

	// Create a new entity
	RenderableComponent c;
	c.header = "Hello World";
	c.text = "This is a component";
	EntityType e = 0;
	rs.addComponent(e, &c);
	
	// Create another entity
	RenderableComponent c2;
	c2.header = "Another Header";
	c2.text = "Blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah blah";
	EntityType e2 = 1;
	rs.addComponent(e2, &c2);

	// Send a message
	ChangeManager::get().recieveMsg<UpdateRenderableMessage>("Hello", "World", 0);

	ChangeManager::get().distrubuteMsgs();

	rs.act();

	cin.get();
}