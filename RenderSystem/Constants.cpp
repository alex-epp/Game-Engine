#include "Constants.h" 

// General
const char* Constants::WINDOW_TITLE  = "OpenGL Engine";
int         Constants::WINDOW_WIDTH  = 640;
int         Constants::WINDOW_HEIGHT = 480;
const bool  Constants::FULLSCREEN    = false;

// Rendering
float Constants::FOV = 45.f;
float Constants::NEAR_PLANE = 0.1f;
float Constants::FAR_PLANE = 10'000.f;

// Vertex shader input locations
const int Constants::VERTEX_LOCATION    = 0;
const int Constants::TEXCOORD_LOCATION  = 1;
const int Constants::NORMAL_LOCATION    = 2;
const int Constants::TANGENT_LOCATION   = 3;
const int Constants::BITANGENT_LOCATION = 4;

// MVP matrix
const int Constants::MODEL_MATRIX_LOCATION = 0;

// Per-frame uniform block
const char* Constants::PERFRAME_UNIFORM_NAME  = "FrameData";
const int   Constants::PERFRAME_UNIFORM_INDEX = 0;

// Lighting
const int Constants::MAX_LIGHTS;
 
// Textures
const string Constants::TEXTURE_NAMES[static_cast<int>(TextureType::NumTypes)] = { "material.diffuseTexture", "material.specularTexture" , "material.shininessTexture" , "material.normalsTexture" };
const string Constants::TEXTURE_FLAGS[static_cast<int>(TextureType::NumTypes)] = { "material.diffuseTextureFlag", "material.specularTextureFlag", "material.shininessTextureFlag" , "material.normalsTextureFlag" };

// Colours
const string Constants::COLOUR_NAMES[static_cast<int>(ColourType::NumTypes)] = { "material.ambientColour", "material.diffuseColour", "material.specularColour" };

// Other material attributes
const string Constants::SHININESS_NAME = "material.shininess";
 
// Files
const string Constants::BASIC_SHADER = "basic";