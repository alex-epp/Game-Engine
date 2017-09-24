#include "Mesh.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::render()
{
	program.use();
	mat.activateTextures();
	vao.draw();
}

void Mesh::compileShaders(string filepath, string shader)
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

#include <SFML/Graphics.hpp>

unsigned int TextureMgr::load(const string filename)
{
	if (textures.find(filename) == textures.end()) // If the texture is not in the map
	{
		/*textures[filename] = SOIL_load_OGL_texture(
				filename.c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				0);*/

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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	return textures[filename];
}

map<string, GLuint> TextureMgr::textures;