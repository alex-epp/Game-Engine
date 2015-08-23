#pragma once

#include <glm.hpp>
using glm::vec3;
using glm::vec2;

struct Vertex
{
	vec3 position;
	vec2 texcoord;
	vec3 normal;
	vec3 tangent;
	vec3 bitangent;
};