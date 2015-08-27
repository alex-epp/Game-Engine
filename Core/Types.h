#include <map>
#include <memory>

using namespace std;

namespace core
{
	using EntityType = unsigned int;
	
	//template<typename T> using ComponentContainer = map<EntityType, T*>;
	
	//template<typename T> using pComponentContainer = map<EntityType, T*>*;

#define ComponentContainer(T) map<core::EntityType, T*>
#define pComponentContainer(T) map<core::EntityType, T*>*
}