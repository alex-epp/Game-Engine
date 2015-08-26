#include "Material.h"


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
	textures[index].handle = TextureMgr::loadTexture(filename);
	if (textures[index].handle == 0)
	{
		LOG_ERR("Could not load ", filename);
		return false;
	}

	// Initialize it with some typical params
	glBindTexture(GL_TEXTURE_2D, textures[index].handle);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

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

void Material::addToProgram(ShaderProgram& program)
{
	for (int i = 0; i < static_cast<int>(TextureType::NumTypes); i++)
	{
		program.setUniform(Constants::TEXTURE_FLAGS[i], textures[i].used);
		if (textures[i].used)
		 	program.setUniform(Constants::TEXTURE_NAMES[i], i);
	}
	

	if (!colours[static_cast<int>(ColourType::Ambient)].used) colours[static_cast<int>(ColourType::Ambient)].val = vec3(0); // Default the ambient colour to black

	for (int i = 0; i < static_cast<int>(ColourType::NumTypes); i++)
			program.setUniform(Constants::COLOUR_NAMES[i], colours[i].val); // Defaults to white if the colour is not used

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
