#pragma once
#include "ShaderProgram.h"
#include "VAO.h"
#include "Material.h"
#include "../Core/Logger.h"
#include <memory>

// Stores the vertex data and the shader
class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(Mesh&& other) = default;

	const VAO& getVAO() const { return vao; }
	VAO& getVAO() { return vao; }

private:
	VAO vao;
};