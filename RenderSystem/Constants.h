#pragma once
#include <string>
using namespace std;

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

class Constants
{
public:

	// General
	static const char* WINDOW_TITLE;
	static int         WINDOW_WIDTH;
	static int         WINDOW_HEIGHT;
	static const bool  FULLSCREEN;

	// Rendering
	static float FOV;
	static float NEAR_PLANE;
	static float FAR_PLANE;

	// Vertex shader input locations
	static const int VERTEX_LOCATION;
	static const int TEXCOORD_LOCATION;
	static const int NORMAL_LOCATION;
	static const int TANGENT_LOCATION;
	static const int BITANGENT_LOCATION;

	// MVP matrix
	static const int MODEL_MATRIX_LOCATION;

	// Per-frame uniform block
	static const char* PERFRAME_UNIFORM_NAME;
	static const int   PERFRAME_UNIFORM_INDEX;

	// Lighting
	static const int MAX_LIGHTS = 8;

	// Textures
	static const string TEXTURE_NAMES[static_cast<int>(TextureType::NumTypes)]; // TextureTypes index into this
	static const string TEXTURE_FLAGS[static_cast<int>(TextureType::NumTypes)];

	// Colours
	static const string COLOUR_NAMES[static_cast<int>(ColourType::NumTypes)]; // ColourTypes index into this

	// Other material attributes
	static const string SHININESS_NAME;

	// Files
	static const string BASIC_SHADER;
	static const string SHADER_PATH;
};

