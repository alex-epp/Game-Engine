#include <map>

using namespace std;

namespace core
{
	using EntityType = unsigned int;
#define ComponentContainer(T) map<EntityType, T*>*
}