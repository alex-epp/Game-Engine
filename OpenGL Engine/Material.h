#pragma once
#include <Windows.h>
#include <GL/glew.h>
#include <glm.hpp>
#include <map>
#include <string>
#include "ShaderProgram.h"
#include "Logger.h"
#include "Constants.h"

using namespace std;
using glm::vec3;

// Makes sure no texture is loaded twice
class TextureMgr
{
public:
	static GLuint loadTexture(const string& filename);
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

	Material();
	~Material();

	bool addTexture(const string& filename, TextureType type);
	void addColour(vec3 val, ColourType type);
	void addAttrib(float val, string name);

	void addToProgram(ShaderProgram& program);
	void activateTextures();

private:
	Texture textures[static_cast<int>(TextureType::NumTypes)];
	Colour colours[static_cast<int>(ColourType::NumTypes)];
	vector<Attrib> attribs;
};