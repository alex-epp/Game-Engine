#pragma once
#include "ShaderProgram.h"
#include "VAO.h"
#include "Material.h"
#include <SOIL.h>
#include "Logger.h"
#include <memory>

class Material;

// Stores the vertex data and the shader
class Mesh
{
public:
	Mesh();
	~Mesh();
	Mesh(Mesh&& other) = default;

	void render();
	GLuint getAttribLocation(string name)
	{
		return glGetAttribLocation(program.getHandle(), name.c_str());
	}

	void compileShaders(string filepath, string shader);

	const ShaderProgram& getProgram() const { return program; }
	ShaderProgram& getProgram() { return program; }

	const VAO& getVAO() const { return vao; }
	VAO& getVAO() { return vao; }
	
	const Material& getMaterial() const { return mat; }
	Material& getMaterial() { return mat; }

	
private:
	friend class Model;
	ShaderProgram program;
	VAO vao;
	Material mat;
};