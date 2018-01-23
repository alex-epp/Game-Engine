#pragma once

#include <tuple>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Message.h"

using namespace std;

namespace core
{
	struct TransformComponent
	{
		TransformComponent(glm::vec3 p = { 0, 0, 0 }, glm::quat r = { 0, 0, 0, 1}) : position(p), rotation(r) {}

		glm::vec3 position;
		glm::quat rotation;
	};

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
