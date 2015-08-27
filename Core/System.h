#pragma once

#include <tuple>
#include <algorithm>

#include "Message.h"

using namespace std;

namespace core
{
	template <class... ComponentsLists>
	class System : public Listener
	{
	protected:
		time_t nextAct;
		tuple<ComponentsLists...> components = {};

	public:
		void setComponents(ComponentsLists... c)
		{
			components = std::forward_as_tuple(c...);
		}
		virtual void act() = 0;
		virtual ~System() = default;
	};
}