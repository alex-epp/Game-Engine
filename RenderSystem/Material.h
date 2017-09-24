#pragma once
#include <future>
#include <Windows.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include "ShaderProgram.h"
#include "../Core/Logger.h"

using namespace std;
using glm::vec3;

// Makes sure no texture is loaded twice
class TextureMgr
{
public:
	static GLuint load(const string filename);
private:
	static map<string, GLuint> textures;
};

class Material
{
public:

	struct Texture
	{
		Texture() : used(false), handle(0) {}
		GLuint handle;
		unsigned int futureHandleRef;
		bool used;
	};
	struct Colour
	{
		Colour() : val(1.f), used(false) {}
		vec3 val;
		bool used;
	};
	struct Attrib
	{
		Attrib(float v, string n) : val(v), name(n) {}
		float val;
		string name;
	};
	enum class TextureType // Texture indices in the shaders
	{
		Diffuse = 0,
		Specular,
		Shininess,
		Normals,
		NumTypes
	};

	enum class ColourType
	{
		Ambient = 0,
		Diffuse,
		Specular,
		NumTypes
	};
	Material();
	~Material();

	bool addTexture(const string& filename, TextureType type);
	void addColour(vec3 val, ColourType type);
	void addAttrib(float val, string name);

	void addToProgram();
	void activateTextures();

	const ShaderProgram& getProgram() const { return program; }
	ShaderProgram& getProgram() { return program; }

	GLuint getAttribLocation(string name)
	{
		return glGetAttribLocation(program.getHandle(), name.c_str());
	}

	void compileShaders(string filepath, string shader);

private:
	Texture textures[static_cast<int>(TextureType::NumTypes)];
	Colour colours[static_cast<int>(ColourType::NumTypes)];
	vector<Attrib> attribs;

	ShaderProgram program;
};