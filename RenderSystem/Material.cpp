#include "Material.h"
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

bool Material::finishTextureLoading()
{
	bool error = false;
	for (auto& tex : textures)
	{
		
	}

	return !error;
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

void Material::addToProgram(ShaderProgram& program)
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
