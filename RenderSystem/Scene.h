#pragma once

#include <tuple>
#include <memory>
#include <vector>

#include "Material.h"
#include "Mesh.h"

class Scene
{
public:
	template <typename... Args>
	shared_ptr<Material> addMaterial(Args... args)
	{
		m_materials.emplace_back(make_shared<Material>(forward(args...)));
		return materials.back();
	}

	template <typename... Args>
	shared_ptr<Mesh> addMesh(Args... args)
	{
		m_meshes.emplace_back(make_shared<Mesh>(forward(args...)));
		return materials.back();
	}

	template <typename... Args>
	shared_ptr<Mesh> addModel(Args... args)
	{
		m_models.emplace_back(make_shared<Model>(forward(args...)));
		return materials.back();
	}

	// Where models, materials, etc. are actually stored
	std::vector<shared_ptr<Model>> m_models;
	std::vector<shared_ptr<Mesh>> m_meshes;
	std::vector<shared_ptr<Material>> m_materials;
};