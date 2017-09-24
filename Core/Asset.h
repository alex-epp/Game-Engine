#if 0

#pragma once

#include <GL/glew.h>
#include <future>
#include <map>
#include <string>
#include <vector>

class AssetManager
{
public:
	enum class AssetType
	{
		Texture, 
	};
	
	AssetManager& get()
	{
		static AssetManager instance;
		return instance;
	}

	unsigned int index(std::string filename)
	{
		if (m_indexes.find(filename) == m_indexes.end())
		{
			static unsigned int index = 0;
			m_indexes[filename] = index++;
		}
		return m_indexes[filename];
	}

	unsigned int loadAsset(std::string filename)
	{
		return loadAsset(index(filename));
	}
	unsigned int loadAsset(unsigned int index)
	{

	}

private:
	AssetManager() = default;

	std::map<std::string, unsigned int> m_indexes;
	std::map<unsigned int, GLuint> m_assets;
	std::map<unsigned int, std::future<GLuint>> m_futures;
};

#endif