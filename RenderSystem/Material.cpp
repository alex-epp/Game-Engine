#include "Material.h"
#include <glbinding/gl/gl.h>
#include <SFML/Graphics.hpp>
#include "../Core/Constants.h"
using core::Constants;

Material::Material()
{
}

Material::~Material()
{
}

bool Material::addTexture(const string& filename, TextureType type)
{
	// Load the texture
	int index = static_cast<int>(type);
	textures[index].handle = TextureMgr::load(filename);
	if (textures[index].handle == 0)
	{
		LOG_ERR("Could not load: ", filename);
		return false;
	}
	textures[index].used = true;

	return true;
}

void Material::addColour(vec3 val, ColourType type)
{
	int index = static_cast<int>(type);
	colours[index].val = val;
	colours[index].used = true;
}

void Material::addAttrib(float val, string name)
{
	attribs.emplace_back(val, name);
}

void Material::compileShaders(string filepath, string shader)
{
	if (!program.compileFromFile(filepath + shader + ".vert", ShaderType::Vertex))
	{
		LOG_ERR("Could not load ", shader, ".vert: ", program.log());
	}
	if (!program.compileFromFile(filepath + shader + ".frag", ShaderType::Fragment))
	{
		LOG_ERR("Could not load ", shader, ".frag: ", program.log());
	}
	/*if (!program.link())
	{
	LOG_ERR("Could not link ", shader, " ", program.log());
	}*/
}

void Material::addToProgram()
{
	static const auto& textureFlags = Constants::get().getStringArray("texture_flags");
	static const auto& textureNames = Constants::get().getStringArray("texture_names");
	static const auto& colourNames = Constants::get().getStringArray("colour_names");

	for (int i = 0; i < static_cast<int>(TextureType::NumTypes); i++)
	{
		program.setUniform(textureFlags[i], textures[i].used);
		if (textures[i].used)
		 	program.setUniform(textureNames[i], i);
	}
	

	if (!colours[static_cast<int>(ColourType::Ambient)].used) colours[static_cast<int>(ColourType::Ambient)].val = vec3(0); // Default the ambient colour to black

	for (int i = 0; i < static_cast<int>(ColourType::NumTypes); i++)
			program.setUniform(colourNames[i], colours[i].val); // Defaults to white if the colour is not used

	for (auto &attrib : attribs)
	{
	 	program.setUniform(attrib.name, attrib.val);
	}
}

void Material::activateTextures()
{
	for (int i = 0; i < static_cast<int>(TextureType::NumTypes); i++)
	{
		if (textures[i].used)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, textures[i].handle);
		}
	}
}

unsigned int TextureMgr::load(const string filename)
{
	if (textures.find(filename) == textures.end()) // If the texture is not in the map
	{
		sf::Image img;
		if (!img.loadFromFile(filename))
		{
			return 0;
		}
		GLuint handle;
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			img.getSize().x, img.getSize().y,
			0,
			GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
		textures[filename] = handle;

		// Initialize it with some typical params
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	return textures[filename];
}

map<string, GLuint> TextureMgr::textures;