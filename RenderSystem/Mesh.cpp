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


GLuint TextureMgr::loadTexture(const string& filename)
{
	if (textures.find(filename) == textures.end()) // If the texture is not in the map
	{
		// Load it
		GLuint texture = SOIL_load_OGL_texture
			(
				filename.c_str(),
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS
				);

		// Store it for later
		textures[filename] = texture;
		return texture;
	}
	else
	{
		return textures[filename];
	}
}

map<string, GLuint> TextureMgr::textures;